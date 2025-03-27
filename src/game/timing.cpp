#include "game/timing.h" // Header
#include <fstream>
#include <ranges>

#include "rule/file_format.h"
#include "utilities.h"

using namespace tfir;

//! UninheritedPoint
void Timing::UninheritedPoint::read(const std::vector<std::string>& content)
{
	time = std::stoi(content[0]);
	beat_length = std::stod(content[1]);
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
	velocity = (-std::stod(content[1]));
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

//! TimingPoint
void Timing::TimingPoint::read(const std::string& line)
{
	const auto content = Utilities::String::split(line, AND);
	if (content.size() < MINIMUM_NUM_CONTENT) return; //TODO: WARNING

	std::visit([&](auto& obj) { obj.read(content); }, *this);
}

//! TimingPoints
void Timing::TimingPoints::read(const std::vector<std::string>& contents)
{
	for (const auto& line : contents)
	{
		const TimingPoint point{ line };

		const auto back_itr = empty() ? end() : std::prev(end());
		emplace_hint(back_itr, std::visit([](const auto& obj) -> int32_t { return obj.time; }, point), point);
	}
}
void Timing::TimingPoints::write(std::ofstream& writer) const
{
	writer << ::TimingPoints::HEADER << '\n';
	for (const auto& point : *this | std::views::values)
		std::visit([&writer](const auto& point) {point.write(writer); }, point);

	writer << '\n';
}