#include "structures/game/beatmap/beatmap.h" // Header
#include <fstream>
#include <ranges>
#include <unordered_map>
#include "format/file.h"
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "utilities.hpp"

namespace Structures::Game::Beatmap
{
	static constexpr int32_t MINIMUM_LINE_CHARACTERS = 3;
	static std::unordered_map<std::string, std::vector<std::string>> read_content(std::ifstream& reader)
	{
		std::unordered_map<std::string, std::vector<std::string>> sections;

		std::string current_line;
		std::getline(reader, current_line);
		while (std::getline(reader, current_line))
		{
			current_line = Utilities::String::trim(current_line);
			static std::string current_section;

			// Nếu current_line là một Header của section
			if (!current_line.empty() && current_line.front() == '[' && current_line.back() == ']')
				current_section = Utilities::String::trim(current_line);
			// Bỏ qua những line không đủ kí tự (điều kiện cần)
			else if (current_line.size() < MINIMUM_LINE_CHARACTERS) {}
			else
				// Những line còn lại là content, sẽ được thêm vào section hiện tại
				sections[current_section].push_back(current_line);
		}
		return sections;
	}
	static void parse_beatmap(Beatmap& beatmap, const std::unordered_map<std::string, std::vector<std::string>>& sections)
	{
		for (const auto& [header, contents] : sections)
		{
			if (header == Format::FileFormat::Beatmap::General::HEADER) beatmap.general.read(contents);
			else if (header == Format::FileFormat::Beatmap::Metadata::HEADER) beatmap.metadata.read(contents);
			else if (header == Format::FileFormat::Beatmap::Difficulty::HEADER) beatmap.calculated_difficulty.apply(Metadata::Difficulty(contents));
			else if (header == Format::FileFormat::Beatmap::HitObjects::HEADER) beatmap.hit_objects.read(contents);
			else if (header == Format::FileFormat::Beatmap::TimingPoints::HEADER) beatmap.timing_points.read(contents);
		}
	}

	// Beatmap::Stats
	unsigned long Beatmap::Stats::get_total_objects_num() const { return count.floor + count.slider; }
	void Beatmap::Stats::calculate(const Beatmap& beatmap)
	{
		// tính thời gian
		time.start_playing_time = beatmap.hit_objects.begin()->second.get_time();
		time.start_time = time.start_playing_time; // tạm thời là vậy đến khi làm xong storyboard
		time.length = beatmap.hit_objects.rbegin()->second.get_end_time() - time.start_playing_time;

		// đếm số object
		auto [floor, slider] = beatmap.make_action_queue();
		while (!floor.empty())
		{
			floor.pop();
			count.floor++;
			count.total_combo++;
		}
		while (!slider.empty())
		{
			const auto current = Utilities::Container::get_front_and_pop(slider);
			count.slider++;
			count.total_combo += current->tick_num;
		}
	}

	// Beatmap
	std::pair<Beatmap::FloorActionQueue, Beatmap::SliderActionQueue> Beatmap::make_action_queue() const
	{
		FloorActionQueue floor_queue;
		SliderActionQueue slider_queue;

		auto previous_direction = Types::Game::Direction::Direction::RIGHT;
		const HitObjects::HitObject* current_hit_object;
		float current_timing_point_velocity;
		float current_beat_length;

		auto if_floor = [&]()
			{
				const HitObjects::Floor::Action current_floor_action{ *std::get_if<HitObjects::Floor>(current_hit_object), previous_direction };
				floor_queue.emplace(std::make_shared<const HitObjects::Floor::Action>(current_floor_action));
				previous_direction += current_hit_object->get_rotation();
			};

		auto if_slider = [&]()
			{
				const HitObjects::Slider::Action current_slider_action{
					*std::get_if<HitObjects::Slider>(current_hit_object), current_timing_point_velocity,
					calculated_difficulty.velocity.speed, current_beat_length
				};
				slider_queue.emplace(std::make_shared<const HitObjects::Slider::Action>(current_slider_action));
				previous_direction += current_hit_object->get_rotation();
			};

		for_all_hit_objects(if_floor, if_slider, current_hit_object, current_timing_point_velocity, current_beat_length);
		return { std::move(floor_queue), std::move(slider_queue) };
	}
	void Beatmap::for_all_hit_objects(const HitObjectStepFunction& if_floor, const HitObjectStepFunction& if_slider,
		const HitObjects::HitObject*& current_hit_object_ptr) const
	{
		for (const auto& hit_object : hit_objects | std::views::values)
		{
			current_hit_object_ptr = &hit_object;

			switch (hit_object.get_type())
			{
			case Types::Game::HitObject::HitObjectType::FLOOR:
				if_floor();
				break;
			case Types::Game::HitObject::HitObjectType::SLIDER:
				if_slider();
				break;
			}
		}
	}
	void Beatmap::for_all_hit_objects(const HitObjectStepFunction& if_floor, const HitObjectStepFunction& if_slider,
		const HitObjects::HitObject*& current_hit_object_ptr,
		float& current_timing_point_velocity, float& current_beat_length) const
	{
		auto [inherited, uninherited] = timing_points.split_to_queue();

		current_timing_point_velocity = 1;
		current_beat_length = 180;
		for (const auto& hit_object : hit_objects | std::views::values)
		{
			current_hit_object_ptr = &hit_object;

			// update timing point
			while (!inherited.empty() && hit_object.get_time() >= inherited.front()->get_time())
				current_timing_point_velocity = Utilities::Container::get_front_and_pop(inherited)->get_velocity();
			while (!uninherited.empty() && hit_object.get_time() >= uninherited.front()->get_time())
				current_beat_length = Utilities::Container::get_front_and_pop(uninherited)->beat_length;

			// do obj
			switch (hit_object.get_type())
			{
			case Types::Game::HitObject::HitObjectType::FLOOR:
				if_floor();
				break;
			case Types::Game::HitObject::HitObjectType::SLIDER:
				if_slider();
				break;
			}
		}
	}
	Beatmap::Beatmap(const char* file_path)
	{
		std::ifstream reader(file_path);
		if (!reader)
			THROW_ERROR(Logging::Exceptions::FileExceptions::File_Open_Failed(file_path));

		parse_beatmap(*this, read_content(reader));
		reader.close();

		stats.calculate(*this);
	}
}