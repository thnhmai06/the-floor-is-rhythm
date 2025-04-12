#include "structures/game/beatmap/timing.h" // Header
#include <fstream>
#include <ranges>
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "format/file.h"
#include "utilities.h"

using namespace Format::FileFormat::Beatmap::TimingPoints;
using Format::FileFormat::Beatmap::AND;

namespace Structures::Game::Beatmap::TimingPoints
{
	//! TimingPoint
	void TimingPoint::read(const std::vector<std::string>& content)
	{
		time = std::stoi(content[0]);
		beat_length = std::stof(content[1]);
		sample_set = static_cast<Hitsound::SampleSetType>(std::stoi(content[2]));
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

			const auto back_itr = empty() ? end() : std::prev(end());
			TimingPoint point(content);
			emplace_hint(back_itr, point.time, point);
		}
	}
	void TimingPoints::write(std::ofstream& writer) const
	{
		writer << HEADER << '\n';
		for (const auto& point : *this | std::views::values)
			point.write(writer);
		writer << '\n';
	}
}