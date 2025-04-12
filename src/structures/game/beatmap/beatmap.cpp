#include "structures/game/beatmap/beatmap.h" // Header
#include <fstream>
#include <ranges>
#include <unordered_map>
#include "format/file.h"
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "utilities.h"

namespace Structures::Game::Beatmap
{
	using Metadata::Difficulty;
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
			else if (header == Format::FileFormat::Beatmap::Difficulty::HEADER) beatmap.calculated_difficulty.apply(Difficulty(contents));
			else if (header == Format::FileFormat::Beatmap::HitObjects::HEADER) beatmap.hit_objects.read(contents);
			else if (header == Format::FileFormat::Beatmap::TimingPoints::HEADER) beatmap.timing_points.read(contents);
		}
	}

	Beatmap::Beatmap(const char* file_path)
	{
		std::ifstream reader(file_path);
		if (!reader)
			THROW_ERROR(Logging::Exceptions::FileExceptions::File_Open_Failed(file_path));

		parse_beatmap(*this, read_content(reader));
		reader.close();

		float current_beat_length = 180;
		float current_timing_velocity = 1.0f;
		auto next_timing_point = timing_points.begin();
		for (const auto& hit_object : hit_objects | std::views::values)
		{
			// lấy timing point
			while (next_timing_point != timing_points.end() &&
				hit_object.get_time() >= next_timing_point->first)
			{
				if (next_timing_point->second.beat_length < 0)
					// inherited
					current_timing_velocity = next_timing_point->second.get_velocity();
				else
					// uninherited
					current_beat_length = next_timing_point->second.beat_length;
				++next_timing_point;
			}

			// tính hit_object count
			switch (hit_object.get_type())
			{
			case Types::Game::HitObject::HitObjectType::FLOOR:
				total_floor++;
				total_combo++;
				break;
			case Types::Game::HitObject::HitObjectType::SLIDER:
				total_slider++;
				{
					const auto current_slider = std::get_if<HitObjects::Slider>(&hit_object);
					const auto time_length = current_slider->end_time - current_slider->time;
					const auto speed = current_timing_velocity * calculated_difficulty.velocity.value;
					float tick_num;
					if (const float tick_spacing_num = static_cast<float>(time_length) * speed / current_beat_length;
						Utilities::Math::is_integer(tick_spacing_num)) tick_num = Utilities::Math::max_float(0, tick_spacing_num - 1);
					else tick_num = std::floor(tick_spacing_num);
					total_combo += static_cast<unsigned long>(tick_num) + 2; // 2 = Đầu Slider + Cuối Slider	
				}
				break;
			}
		}
		total_objects_num = total_floor + total_slider;
	}
}