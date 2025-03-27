#include "game/hitobject.h" // Header
#include <fstream>
#include <ranges>
#include "rule/file_format.h"
#include "utilities.h"

using namespace tfir_file::Beatmap::HitObjects;
using tfir_file::Beatmap::AND;

Direction HitObject::get_next_direction(const Direction prev_direction, const uint8_t direction_jump)
{
	return static_cast<Direction>((static_cast<uint8_t>(prev_direction) + direction_jump) % NUM_DIRECTIONS);
}

//! Floor
void HitObject::Floor::read(const std::vector<std::string>& content)
{
	time = std::stoi(content[0]);
	direction_jump = std::stoi(content[1]);
	combo_jump = std::stoi(content[2]);
	hitsound = Hitsound::Hitsound{ std::stoi(content[4]) };
	hitsample = Hitsound::HitSample{ content[5] };
}
void HitObject::Floor::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(direction_jump) << AND << static_cast<int32_t>(combo_jump) << AND <<
		static_cast<bool>(HitObjectType::FLOOR);
	writer << AND << hitsound.to_int() << AND << hitsample.to_string();
	writer << '\n';
}

//! Slider
void HitObject::Slider::read(const std::vector<std::string>& content)
{
	time = std::stoi(content[0]);
	direction_jump = std::stoi(content[1]);
	combo_jump = std::stoi(content[2]);
	end_time = std::stoi(content[4]);

	// Curves
	for (const auto curves_str = Utilities::String::split(content[5], ::Slider::AND); const auto & curve : curves_str)
	{
		const auto curve_str = Utilities::String::split(curve, ::Slider::CURVE_AND, true);
		curves.push_back({
			.add_time = std::stoi(curve_str[0]),
			.direction_jump = static_cast<uint8_t>(std::stoi(curve_str[1]))
			});
	}
	hitsound = Hitsound::Hitsound{ std::stoi(content[6]) };
	hitsample = Hitsound::HitSample{ content[7] };
}
void HitObject::Slider::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(direction_jump) << AND << static_cast<int32_t>(combo_jump) << AND <<
		static_cast<bool>(HitObjectType::SLIDER) << AND << end_time << AND;
	for (auto ptr = curves.begin(); ptr != curves.end(); ++ptr) {
		if (ptr != curves.begin()) writer << tfir_file::Beatmap::HitObjects::Slider::AND;
		writer << ptr->add_time << tfir_file::Beatmap::HitObjects::Slider::CURVE_AND << static_cast<int32_t>(ptr->direction_jump);
	}
	writer << AND;
	writer << hitsound.to_int() << AND << hitsample.to_string();
	writer << '\n';
}

//! HitObject
void HitObject::HitObject::read(const std::string& line)
{
	const auto content = Utilities::String::split(line, AND);
	if (content.size() < MINIMUM_NUM_CONTENT) return //TODO: Log Warning

		std::visit([&](auto& obj) { obj.read(content); }, *this);
}

//! HitObjects
void HitObject::HitObjects::read(const std::vector<std::string>& contents)
{
	for (const auto& line : contents)
	{
		const HitObject obj{ line };

		const auto back_itr = empty() ? end() : std::prev(end());
		emplace_hint(back_itr, std::visit([](const auto& obj) -> int32_t { return obj.time; }, obj), obj);
	}
}
void HitObject::HitObjects::write(std::ofstream& writer) const
{
	writer << HEADER << '\n';
	for (const auto& obj : *this | std::views::values)
		std::visit([&writer](const auto& obj) { obj.write(writer); }, obj);
	writer << '\n';
}
