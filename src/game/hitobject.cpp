#include "game/hitobject.h" // Header
#include <fstream>
#include <ranges>
#include "rule/file_format.h"
#include "utilities.h"

using namespace FileFormat::Beatmap::HitObjects;
using FileFormat::Beatmap::AND;

Direction HitObject::get_next_direction(const Direction prev_direction, const uint8_t direction_jump)
{
	return static_cast<Direction>((static_cast<uint8_t>(prev_direction) + direction_jump) % NUM_DIRECTIONS);
}

//! Floor
void HitObject::Floor::read(const std::vector<std::string>& content)
{
	end_time = time = std::stoi(content[0]);
	direction_jump = static_cast<DirectionJump>(std::stoi(content[1]));
	combo_jump = std::stoi(content[2]);
	hitsound = Hitsound::Hitsound{ std::stoi(content[4]) };
	hitsample = Hitsound::HitSample{ content[5] };
}
void HitObject::Floor::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(direction_jump) << AND << static_cast<int32_t>(combo_jump) << AND <<
		static_cast<bool>(type);
	writer << AND << hitsound.to_int() << AND << hitsample.to_string();
	writer << '\n';
}

//! Slider
void HitObject::Slider::read(const std::vector<std::string>& content)
{
	time = std::stoi(content[0]);
	direction_jump = static_cast<DirectionJump>(std::stoi(content[1]));
	combo_jump = std::stoi(content[2]);
	end_time = std::stoi(content[4]);

	// Curves
	for (const auto curves_str = Utilities::String::split(content[5], ::Slider::AND); const auto & curves : curves_str)
	{
		const auto curve_str = Utilities::String::split(curves, ::Slider::CURVE_AND, true);
		const SliderCurve curve = {std::stoi(curve_str[0]), static_cast<DirectionJump>(std::stoi(curve_str[1]))};
		this->curves.push_back(curve);
	}
	hitsound = Hitsound::Hitsound{ std::stoi(content[6]) };
	hitsample = Hitsound::HitSample{ content[7] };
}
void HitObject::Slider::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(direction_jump) << AND << static_cast<int32_t>(combo_jump) << AND <<
		static_cast<bool>(type) << AND << end_time << AND;
	for (auto ptr = curves.begin(); ptr != curves.end(); ++ptr) {
		if (ptr != curves.begin()) writer << FileFormat::Beatmap::HitObjects::Slider::AND;
		writer << ptr->padding_time << FileFormat::Beatmap::HitObjects::Slider::CURVE_AND << static_cast<int32_t>(ptr->direction_jump);
	}
	writer << AND;
	writer << hitsound.to_int() << AND << hitsample.to_string();
	writer << '\n';
}

//! HitObjects
void HitObject::HitObjects::read(const std::vector<std::string>& contents)
{
	for (const auto& line : contents)
	{
		const auto content = Utilities::String::split(line, AND);
		if (content.size() < HitObject::MINIMUM_NUM_CONTENT) continue; // TODO: Warning

		const auto back_itr = (empty()) ? end() : std::prev(end());
		switch (std::stoi(content[3]))
		{
		case static_cast<int32_t>(HitObjectType::FLOOR):
		{
			Floor floor;
			floor.read(content);
			this->emplace_hint(back_itr, floor.time, std::make_unique<Floor>(std::move(floor)));
			break;
		}
		case static_cast<int32_t>(HitObjectType::SLIDER):
		{
			Slider slider;
			if (content.size() < Slider::MINIMUM_NUM_CONTENT) continue; //TODO: Warning
			slider.read(content);
			this->emplace_hint(back_itr, slider.time, std::make_unique<Slider>(std::move(slider)));
			break;
		}
		default:
			//TODO: Warning
			break;
		}
	}
}
void HitObject::HitObjects::write(std::ofstream& writer) const
{
	writer << HEADER << '\n';
	for (const auto& hitobject : *this | std::views::values)
		hitobject->write(writer);
	writer << '\n';
}
