#include "structures/game/mapset/hitobject.h" // Header
#include <fstream>
#include <ranges>
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "format/file.h"
#include "utilities.hpp"

namespace Structures::Game::Beatmap::HitObjects
{
	using Format::File::Floor::Mapset::AND;

	//! Floor
	void Floor::read(const std::vector<std::string>& contents)
	{
		time = std::stof(contents[0]);
		is_kat = contents[1] == "1";
		hit_sound = Hitsound::HitSound{ std::stoi(contents[2]) };
		hit_sample = Hitsound::HitSample{ contents[3] };
	}
	std::string Floor::to_string() const
	{
		std::stringstream ss;
		ss << time << AND << is_kat
			<< AND << hit_sound.to_int() << AND << hit_sample.to_string();
		return ss.str();
	}
	Floor::Floor(const std::vector<std::string>& content) { read(content); }

	//! HitObjects
	void HitObjects::read(const std::vector<std::string>& lines)
	{
		data.clear();
		for (const auto& line : lines)
		{
			const auto contents = Utilities::String::split(line, AND);
			if (contents.size() < Floor::MINIMUM_NUM_CONTENT)
			{
				LOG_WARNING(Logging::Exceptions::FormatExceptions::HitObjects::Format_HitObjects_NotEnoughContent(line));
				continue;
			}

			Floor it{ contents };
			data.emplace_hint(
				Utilities::Container::get_last_element_iterator(data), 
				it.time, std::move(it));
		}
	}
	std::string HitObjects::to_string() const
	{
		std::stringstream ss;
		ss << Format::File::Floor::Mapset::HitObjects::HEADER;

		for (const auto& hit_object : data | std::views::values)
			ss << '\n' << hit_object.to_string();
		return ss.str();
	}
	std::ostream& operator<<(std::ostream& os, const HitObjects& hit_objects)
	{
		os << hit_objects.to_string();
		return os;
	}
}