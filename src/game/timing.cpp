#include "game/timing.h" // Header
#include <fstream>
#include <ranges>
#include "rule/file_format.h"
#include "utilities.h"

using namespace FileFormat::Beatmap::TimingPoints;
using FileFormat::Beatmap::HitObjects::Slider::AND;

//! UninheritedPoint
void Timing::UninheritedPoint::read(const std::vector<std::string>& content)
{
	time = std::stoi(content[0]);
	beat_length = std::stof(content[1]);
	meter = std::stoi(content[2]);
	sample_set = static_cast<Hitsound::SampleSetType>(std::stoi(content[3]));
	sample_index = std::stoi(content[4]);
	volume = std::stoi(content[5]);
	kiai = std::stoi(content[6]);
}
void Timing::UninheritedPoint::write(std::ofstream& writer) const
{
	writer << time << AND << beat_length << AND << meter << AND << static_cast<int32_t>(sample_set) << AND <<
		sample_index << AND << volume << AND << kiai << '\n';
}

//! InheritedPoint
void Timing::InheritedPoint::read(const std::vector<std::string>& content)
{
	time = std::stoi(content[0]);
	velocity = (-std::stof(content[1]));
	meter = std::stoi(content[2]);
	sample_set = static_cast<Hitsound::SampleSetType>(std::stoi(content[3]));
	sample_index = std::stoi(content[4]);
	volume = std::stoi(content[5]);
	kiai = std::stoi(content[6]);
}
void Timing::InheritedPoint::write(std::ofstream& writer) const
{
	writer << time << AND << (-velocity) << AND << meter << AND << static_cast<int32_t>(sample_set) << AND <<
		sample_index << AND << volume << AND << kiai << '\n';
}

//! TimingPoints
void Timing::TimingPoints::read(const std::vector<std::string>& contents)
{
	for (const auto& line : contents)
	{
		const auto content = Utilities::String::split(line, AND);
		if (content.size() < TimingPoint::MINIMUM_NUM_CONTENT) continue; // TODO: Warning

		const auto back_itr = empty() ? end() : std::prev(end());
		if (std::stod(content[1]) >= 0)
		{
			UninheritedPoint point(content);
			emplace_hint(back_itr, point.time, std::make_unique<UninheritedPoint>(std::move(point)));
		}
		else
		{
			InheritedPoint point(content);
			emplace_hint(back_itr, point.time, std::make_unique<InheritedPoint>(std::move(point)));
		}
	}
}
void Timing::TimingPoints::write(std::ofstream& writer) const
{
	writer << HEADER << '\n';
	for (const auto& point : *this | std::views::values)
		point->write(writer);
	writer << '\n';
}