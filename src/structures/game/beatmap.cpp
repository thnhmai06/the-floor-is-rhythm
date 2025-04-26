#include "structures/game/beatmap.h" // Header
#include <fstream>
#include <ranges>
#include <unordered_map>
#include "format/file.h"
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "utilities.hpp"

namespace Structures::Game::Beatmap
{
	static constexpr size_t MINIMUM_LINE_CHARACTERS = 3;
	static std::unordered_map<std::string, std::vector<std::string>> read_content(std::ifstream& reader)
	{
		std::unordered_map<std::string, std::vector<std::string>> sections;

		std::string current_line;
		std::string current_section = {};
		
		while (std::getline(reader, current_line))
		{
			if (current_section == Format::File::Floor::Mapset::Events::HEADER)
			{
				current_line = Utilities::String::trim(current_line, true);
				const auto current_line_if_trim_all = Utilities::String::trim(current_line);

				if (current_line_if_trim_all.size() < MINIMUM_LINE_CHARACTERS) continue;
				if (current_line_if_trim_all[0] == '/' && current_line_if_trim_all[1] == '/') continue; // comment
			}
			else
			{
				current_line = Utilities::String::trim(current_line);
				if (current_line.size() < MINIMUM_LINE_CHARACTERS) continue;
				if (current_line[0] == '/' && current_line[1] == '/') continue; // comment
			}

			if (!current_line.empty() && current_line.front() == '[' && current_line.back() == ']')
			{
				current_section = Utilities::String::trim(current_line);
				continue;
			}
			
			sections[current_section].push_back(current_line);
		}
		return sections;
	}

	//! Storyboard
	void Storyboard::parse(std::unordered_map<std::string, std::vector<std::string>>& sections)
	{
		for (auto& [header, contents] : sections)
		{
			if (header == Format::File::Floor::Mapset::Variables::HEADER) variables.Parse(contents);
			else if (header == Format::File::Floor::Mapset::Events::HEADER) events.Parse(contents, variables);
		}
	}
	Storyboard::Storyboard(const std::filesystem::path& file)
	{
		std::ifstream reader(file);
		if (!reader)
			THROW_ERROR(Logging::Exceptions::FileExceptions::File_Open_Failed(file));
		auto sections = read_content(reader);
		reader.close();

		parse(sections);
	}

	//! Mapset
	// Mapset::Stats
	unsigned long Mapset::Stats::get_total_objects_num() const { return count.floor + count.slider; }
	void Mapset::Stats::calculate(const Mapset& beatmap)
	{
		// tính thời gian
		time.start_playing_time = beatmap.hit_objects.data.begin()->second->time;
		time.start_time = time.start_playing_time; // tạm thời là vậy đến khi làm xong storyboard
		time.length = beatmap.hit_objects.data.rbegin()->second->end_time - time.start_playing_time;

		// đếm số object
		for (const auto& obj : beatmap.hit_objects.data | std::views::values)
		{
			switch (obj->type_data.type)
			{
			case Types::Game::HitObject::HitObjectType::Floor: count.floor++;
			case Types::Game::HitObject::HitObjectType::Slider: count.slider++;
			}
		}
	}

	// Mapset
	//void Mapset::for_in_hit_objects(const HitObjectStepFunction& if_floor, const HitObjectStepFunction& if_slider,
	//	const HitObjects::HitObject*& current_hit_object_ptr,
	//	float& current_timing_point_velocity, float& current_beat_length) const
	//{
	//	auto [inherited, uninherited] = timing_points.split_to_queue();

	//	current_timing_point_velocity = 1;
	//	current_beat_length = 0.333333f; // 180 bpm
	//	for (const auto& hit_object : hit_objects | std::views::values)
	//	{
	//		current_hit_object_ptr = &hit_object;

	//		// update timing point
	//		while (!inherited.empty() && hit_object.get_time() >= inherited.front()->get_time())
	//			current_timing_point_velocity = Utilities::Container::get_front_and_pop(inherited)->get_velocity();
	//		while (!uninherited.empty() && hit_object.get_time() >= uninherited.front()->get_time())
	//			current_beat_length = Utilities::Container::get_front_and_pop(uninherited)->beat_length;

	//		// do obj
	//		switch (hit_object.get_type())
	//		{
	//		case Types::Game::HitObject::HitObjectType::Floor:
	//			if_floor();
	//			break;
	//		case Types::Game::HitObject::HitObjectType::Slider:
	//			if_slider();
	//			break;
	//		}
	//	}
	//}
	//::
	void Mapset::parse(std::unordered_map<std::string, std::vector<std::string>>& sections)
	{
		for (auto& [header, contents] : sections)
		{
			if (header == Format::File::Floor::Mapset::General::HEADER) general.read(contents);
			else if (header == Format::File::Floor::Mapset::Metadata::HEADER) metadata.read(contents);
			else if (header == Format::File::Floor::Mapset::Difficulty::HEADER) 
				calculated_difficulty = std::make_unique<Metadata::CalculatedDifficulty>(Metadata::Difficulty(contents));
			else if (header == Format::File::Floor::Mapset::HitObjects::HEADER) hit_objects.read(contents);
			else if (header == Format::File::Floor::Mapset::TimingPoints::HEADER) timing_points.read(contents);
			else if (header == Format::File::Floor::Mapset::Events::HEADER) events.Parse(contents);
		}
	}
	Mapset::Mapset(const std::filesystem::path& path)
	{
		std::ifstream reader(path);
		if (!reader)
			THROW_ERROR(Logging::Exceptions::FileExceptions::File_Open_Failed(path));

		auto sections = read_content(reader);
		reader.close();
		parse(sections);
		stats.calculate(*this);
	}
}