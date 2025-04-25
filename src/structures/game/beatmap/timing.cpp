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
	void TimingPoint::write(std::ofstream& writer) const
	{
		writer << time << AND << beat_length << AND << static_cast<int32_t>(sample_set) << AND <<
			sample_index << AND << volume << AND << kiai << '\n';
	}

	int64_t TimingPoint::get_time() const { return time; }
	float TimingPoint::get_velocity() const { return (inherited) ? 100 / (-beat_length) : 1; }

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

			const auto back_itr = Utilities::Container::get_last_element_iterator(*this);
			TimingPoint point(content);
			emplace_hint(back_itr, point.time, point);
		}
	}
	std::pair<std::queue<const TimingPoint*>, std::queue<const TimingPoint*>> TimingPoints::split_to_queue() const
	{
		std::queue<const TimingPoint*> inherited, uninherited;
		for (const auto& point : *this | std::views::values)
		{
			if (point.inherited)
				inherited.push(&point);
			else
				uninherited.push(&point);
		}
		return { inherited, uninherited };
	}
	void TimingPoints::write(std::ofstream& writer) const
	{
		writer << HEADER << '\n';
		for (const auto& point : *this | std::views::values)
			point.write(writer);
		writer << '\n';
	}
}