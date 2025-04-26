#include "structures/game/beatmap/timing.h" // Header
#include <fstream>
#include <ranges>
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "format/file.h"
#include "utilities.hpp"

using namespace Format::File::Floor::Mapset::TimingPoints;
using Format::File::Floor::Mapset::AND;

namespace Structures::Game::Beatmap::TimingPoints
{
	//! TimingPoint
	void TimingPoint::read(const std::vector<std::string>& content)
	{
		time = std::stoi(content[0]);
		beat_length = std::stof(content[1]);
		sample_set = static_cast<Hitsound::HitSampleType>(std::stoi(content[2]));
		sample_index = std::stoi(content[3]);
		volume = std::stoi(content[4]);
		inherited = (beat_length < 0);
		kiai = std::stoi(content[5]);
	}
	int64_t TimingPoint::get_time() const { return time; }
	float TimingPoint::get_velocity() const { return (inherited) ? 100 / (-beat_length) : 1; }
	std::string TimingPoint::to_string() const
	{
		std::stringstream writer;
		writer << time << AND << beat_length << AND << static_cast<int32_t>(sample_set) << AND <<
			sample_index << AND << volume << AND << kiai;
		return writer.str();
	}
	std::ostream& operator<<(std::ostream& os, const TimingPoint& point)
	{
		os << point.to_string();
		return os;
	}

	//! TimingPoints
	void TimingPoints::read(const std::vector<std::string>& contents)
	{
		for (const auto& line : contents)
		{
			const auto content = Utilities::String::split(line, AND);
			if (content.size() < TimingPoint::MINIMUM_NUM_CONTENT)
			{
				LOG_WARNING(Logging::Exceptions::FormatExceptions::TimingPoints::Format_TimingPoints_NotEnoughContent(line));
				continue;
			}

			const auto back_itr = Utilities::Container::get_last_element_iterator(data);
			TimingPoint point(content);
			data.emplace_hint(back_itr, point.time, std::move(point));
		}
	}
	std::pair<std::queue<const TimingPoint*>, std::queue<const TimingPoint*>> TimingPoints::split_to_queue() const
	{
		std::queue<const TimingPoint*> inherited, uninherited;
		for (const auto& point : data | std::views::values)
		{
			if (point.inherited) inherited.push(&point);
			else uninherited.push(&point);
		}
		return { inherited, uninherited };
	}
	std::pair<std::multimap<int64_t, const TimingPoint*>, std::multimap<int64_t, const TimingPoint*>> TimingPoints::split_to_map() const
	{
		std::multimap<int64_t, const TimingPoint*> inherited, uninherited;
		for (const auto& point : data | std::views::values)
		{
			if (point.inherited)
				inherited.emplace_hint(Utilities::Container::get_last_element_iterator(inherited), point.time, &point);
			else
				uninherited.emplace_hint(Utilities::Container::get_last_element_iterator(uninherited), point.time, &point);
		}
		return { inherited, uninherited };
	}
	std::queue<const TimingPoint*> TimingPoints::to_queue() const
	{
		std::queue<const TimingPoint*> res;
		for (const auto& point : data | std::views::values)
			res.push(&point);
		return res;
	}
	std::string TimingPoints::to_string() const
	{
		std::stringstream writer;
		writer << HEADER << '\n';
		for (const auto& point : data | std::views::values) 
			writer << point << '\n';
		return writer.str();
	}
	std::ostream& operator<<(std::ostream& os, const TimingPoints& points)
	{
		return os << points.to_string();
	}
}
