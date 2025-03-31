#include "game/hitobject.h" // Header
#include <fstream>
#include <ranges>
#include "exceptions.h"
#include "logging.h"
#include "rule/file_format.h"
#include "template.h"
#include "utilities.h"

using namespace FileFormat::Beatmap::HitObjects;
using FileFormat::Beatmap::AND;

Template::Game::Direction::Direction HitObject::get_next_direction(const Template::Game::Direction::Direction prev_direction, const uint8_t rotation)
{
	return static_cast<Template::Game::Direction::Direction>((static_cast<uint8_t>(prev_direction) + rotation) % NUM_DIRECTIONS);
}

//! Floor
void HitObject::Floor::read(const std::vector<std::string>& content)
{
	end_time = time = std::stoi(content[0]);
	rotation = static_cast<Template::Game::Direction::Rotation>(std::stoi(content[1]));
	combo_jump = std::stoi(content[2]);
	hitsound = Hitsound::Hitsound{ std::stoi(content[4]) };
	hitsample = Hitsound::HitSample{ content[5] };
}
void HitObject::Floor::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(rotation) << AND << static_cast<int32_t>(combo_jump) << AND <<
		static_cast<bool>(type);
	writer << AND << hitsound.to_int() << AND << hitsample.to_string();
	writer << '\n';
}

//! Slider
void HitObject::Slider::read(const std::vector<std::string>& content)
{
	time = std::stoi(content[0]);
	rotation = static_cast<Template::Game::Direction::Rotation>(std::stoi(content[1]));
	combo_jump = std::stoi(content[2]);
	end_time = std::stoi(content[4]);

	// Curves
	for (const auto curves_str = Utilities::String::split(content[5], ::Slider::AND); const auto & curves : curves_str)
	{
		const auto curve_str = Utilities::String::split(curves, ::Slider::CURVE_AND, true);
		const SliderCurve curve = {std::stoi(curve_str[0]), static_cast<Template::Game::Direction::Rotation>(std::stoi(curve_str[1]))};
		this->curves.push_back(curve);
	}
	hitsound = Hitsound::Hitsound{ std::stoi(content[6]) };
	hitsample = Hitsound::HitSample{ content[7] };
}
void HitObject::Slider::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(rotation) << AND << static_cast<int32_t>(combo_jump) << AND <<
		static_cast<bool>(type) << AND << end_time << AND;
	for (auto ptr = curves.begin(); ptr != curves.end(); ++ptr) {
		if (ptr != curves.begin()) writer << FileFormat::Beatmap::HitObjects::Slider::AND;
		writer << ptr->padding_time << FileFormat::Beatmap::HitObjects::Slider::CURVE_AND << static_cast<int32_t>(ptr->rotation);
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
		if (content.size() < HitObject::MINIMUM_NUM_CONTENT)
		{
			LOG_WARNNING(FormatExceptions::HitObjects::Format_HitObjects_NotEnoughtContent(line));
			continue;
		}

		const auto back_itr = (empty()) ? end() : std::prev(end());
		switch (std::stoi(content[3]))
		{
		case static_cast<int32_t>(Template::Game::HitObject::HitObjectType::FLOOR):
		{
			Floor floor;
			floor.read(content);
			this->emplace_hint(back_itr, floor.time, std::make_unique<Floor>(std::move(floor)));
			break;
		}
		case static_cast<int32_t>(Template::Game::HitObject::HitObjectType::SLIDER):
		{
			Slider slider;
			if (content.size() < Slider::MINIMUM_NUM_CONTENT)
			{
				LOG_WARNNING(FormatExceptions::HitObjects::Format_HitObjects_NotEnoughtContent(line));
				continue;
			}
			slider.read(content);
			this->emplace_hint(back_itr, slider.time, std::make_unique<Slider>(std::move(slider)));
			break;
		}
		default:
			LOG_WARNNING(FormatExceptions::HitObjects::Format_HitObjects_InvalidContent(line));
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
