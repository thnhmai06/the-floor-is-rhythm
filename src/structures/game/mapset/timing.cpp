#include "structures/game/mapset/timing.h" // Header
#include <fstream>
#include <ranges>
#include "config.h"
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
		sample.set = static_cast<Hitsound::SampleSet>(std::stoi(content[2]));
		sample.index = std::stoi(content[3]);
		sample.volume = std::stoi(content[4]);
		inherited = (beat_length < 0);
		kiai = std::stoi(content[5]);
	}
	float TimingPoint::get_velocity() const { return (inherited) ? 100 / (-beat_length) : 1; }
	std::string TimingPoint::to_string() const
	{
		std::stringstream writer;
		writer << time << AND << beat_length << AND << static_cast<int32_t>(sample.set)
			<< AND << sample.index << AND << static_cast<int32_t>(sample.volume) << AND << kiai;
		return writer.str();
	}
	float TimingPoints::get_object_pos(const float& current_time) const
	{
		auto current_timing = timing_pos.upper_bound(current_time);
		if (current_timing == timing_pos.begin()) return 0.0f;
		--current_timing;

		const float time_elapsed = current_time - current_timing->first;
		const auto& [pos, beat_length, velocity] = current_timing->second;
		return pos + ::Config::Game::Difficulty::Velocity::BASE_ONE_BEAT_PIXEL_LENGTH * velocity * time_elapsed / beat_length;
	}
	TimingPoint::TimingPoint(const std::vector<std::string>& content)
	{
		TimingPoint::read(content);
	}

	//! TimingPoints
	void TimingPoints::read(const std::vector<std::string>& contents)
	{
		data.clear();
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
			data.emplace_hint(back_itr, point.time, point);
		}

		float current_beat_length = 60.0f / 180;
		float current_velocity = 1;
		float current_pos = 0;
		int64_t current_time = 0;
		for (const auto& [time, point] : data)
		{
			if (time < 0) continue;
			current_pos +=
				::Config::Game::Difficulty::Velocity::BASE_ONE_BEAT_PIXEL_LENGTH
				* current_velocity * (time - current_time) / current_beat_length;

			current_time = time;
			if (point.inherited)
				current_velocity = point.get_velocity();
			else current_beat_length = point.beat_length;

			timing_pos.emplace_hint(
				Utilities::Container::get_last_element_iterator(timing_pos),
				time, TimingPos{.pos = current_pos, .beat_length = current_beat_length, .velocity = current_velocity});
		}
	}
	std::string TimingPoints::to_string() const
	{
		std::stringstream writer;
		writer << HEADER << '\n';
		for (const auto& point : data | std::views::values)
			writer << point << '\n';
		return writer.str();
	}
	TimingPoints::TimingPoints(const std::vector<std::string>& contents) { TimingPoints::read(contents); }
}
