#include "structures/game/mapset.h" // Header
#include <osu!parser/Parser/Beatmap.hpp>
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
	unsigned long Mapset::Stats::get_total_objects_num() const { return count.floor; }
	void Mapset::Stats::calculate(const Mapset& mapset)
	{
		// tính thời gian
		time.start_playing_time = mapset.hit_objects.data.begin()->first;
		time.start_time = std::min(0LL, time.start_playing_time); // tạm thời là vậy đến khi làm xong storyboard
		time.length = mapset.hit_objects.data.rbegin()->first - time.start_playing_time;

		// đếm số object
		count.floor = count.total_combo = mapset.hit_objects.data.size();
	}

	// Mapset
	void Mapset::parse(std::unordered_map<std::string, std::vector<std::string>>& sections)
	{
		for (auto& [header, contents] : sections)
		{
			if (header == Format::File::Floor::Mapset::General::HEADER) general.read(contents);
			else if (header == Format::File::Floor::Mapset::Metadata::HEADER) metadata.read(contents);
			else if (header == Format::File::Floor::Mapset::Difficulty::HEADER)
				calculated_difficulty = std::make_unique<Metadata::CalculatedDifficulty>(contents);
			else if (header == Format::File::Floor::Mapset::HitObjects::HEADER) hit_objects.read(contents);
			else if (header == Format::File::Floor::Mapset::TimingPoints::HEADER) timing_points.read(contents);
			else if (header == Format::File::Floor::Mapset::Events::HEADER) events.Parse(contents);
		}
	}
	void Mapset::load_storyboard_file()
	{
		std::filesystem::path osb_path;
		for (const auto& entry : std::filesystem::directory_iterator(path.parent_path())) 
		{
			if (entry.path().extension() == Format::File::Osu::STORYBOARD_EXTENSION) 
			{
				osb_path = Utilities::Path::make_valid_long_path(entry.path());
				break;
			}
		}

		if (osb_path.empty()) return;
		OsuParser::Beatmap::Beatmap storyboard{ osb_path.string(), true };
		for (auto& event : storyboard.Events.objects)
			events.objects.push_back(event);
	}
	std::string Mapset::to_string() const
	{
		using namespace Format::File::Floor::Mapset;
		std::stringstream ss;

		ss << FORMAT_VERSION << "\n\n";
		ss << general << "\n\n";
		ss << calculated_difficulty << "\n\n";
		ss << metadata << "\n\n";
		ss << timing_points << "\n\n";
		ss << hit_objects << "\n\n";
		ss << events;

		return ss.str();
	}
	Mapset::Mapset(const fs::path& path, const bool load_storyboard_file)
	: path(Utilities::Path::make_valid_long_path(path))
	{
		std::ifstream reader(this->path);
		if (!reader) 
			THROW_ERROR(Logging::Exceptions::FileExceptions::File_Open_Failed(this->path));
		auto sections = read_content(reader);
		reader.close();
		parse(sections);
		stats.calculate(*this);

		if (load_storyboard_file) this->load_storyboard_file();
	}
}