#include "structures/game/hitobject.h" // Header
#include <fstream>
#include <ranges>
#include "logger/exceptions.h"
#include "logger/logging.h"
#include "format/file.h"
#include "template.h"
#include "utilities.h"

using namespace GameObjects::HitObjects;
using FileFormat::Beatmap::AND;

Template::Game::Direction::Direction GameObjects::HitObjects::get_next_direction(const Template::Game::Direction::Direction& prev_direction, uint8_t rotation)
{
	return static_cast<Template::Game::Direction::Direction>((static_cast<uint8_t>(prev_direction) + rotation) % Template::Game::Direction::NUM_DIRECTIONS);
}

//! Floor
void Floor::read(const std::vector<std::string>& content)
{
	end_time = time = std::stoi(content[0]);
	rotation = static_cast<Template::Game::Direction::Rotation>(std::stoi(content[1]));
	combo_jump = std::stoi(content[2]);
	hit_sound = Hitsound::Hitsound{ std::stoi(content[4]) };
	hit_sample = Hitsound::HitSample{ content[5] };
}
void Floor::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(rotation) << AND << static_cast<int32_t>(combo_jump) << AND <<
		static_cast<bool>(type);
	writer << AND << hit_sound.to_int() << AND << hit_sample.to_string();
	writer << '\n';
}

//! Slider
void Slider::read(const std::vector<std::string>& content)
{
	time = std::stoi(content[0]);
	rotation = static_cast<Template::Game::Direction::Rotation>(std::stoi(content[1]));
	combo_jump = std::stoi(content[2]);
	end_time = std::stoi(content[4]);

	// Curves
	for (const auto curves_str = Utilities::String::split(content[5], FileFormat::Beatmap::HitObjects::Slider::AND);
	     const auto& curves : curves_str)
	{
		const auto curve_str = Utilities::String::split(curves, FileFormat::Beatmap::HitObjects::Slider::CURVE_AND, true);
		const SliderCurve curve = {
			.after = std::stoi(curve_str[0]),
			.rotation = static_cast<Template::Game::Direction::Rotation>(std::stoi(curve_str[1]))
		};
		this->curves.push_back(curve);
	}
	hit_sound = Hitsound::Hitsound{ std::stoi(content[6]) };
	hit_sample = Hitsound::HitSample{ content[7] };
}
void Slider::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(rotation) << AND << static_cast<int32_t>(combo_jump) << AND <<
		static_cast<bool>(type) << AND << end_time << AND;
	for (auto ptr = curves.begin(); ptr != curves.end(); ++ptr) {
		if (ptr != curves.begin()) writer << FileFormat::Beatmap::HitObjects::Slider::AND;
		writer << ptr->after << FileFormat::Beatmap::HitObjects::Slider::CURVE_AND << static_cast<int32_t>(ptr->rotation);
	}
	writer << AND << hit_sound.to_int() << AND << hit_sample.to_string();
	writer << '\n';
}

//! HitObject
int32_t HitObject::get_time() const { return std::visit([](const auto& hit_object) { return hit_object.time; }, *this); }
int32_t HitObject::get_end_time() const { return std::visit([](const auto& hit_object) { return hit_object.end_time; }, *this); }
Template::Game::HitObject::HitObjectType HitObject::get_type() const { return std::visit([](const auto& hit_object) { return hit_object.type; }, *this); }
Template::Game::Direction::Rotation HitObject::get_rotation() const { return std::visit([](const auto& hit_object) { return hit_object.rotation; }, *this); }
uint8_t HitObject::get_combo_jump() const { return std::visit([](const auto& hit_object) { return hit_object.combo_jump; }, *this); }
GameObjects::Hitsound::Hitsound HitObject::get_hitsound() const { return std::visit([](const auto& hit_object) { return hit_object.hit_sound; }, *this); }
GameObjects::Hitsound::HitSample HitObject::get_hitsample() const { return std::visit([](const auto& hit_object) { return hit_object.hit_sample; }, *this); }
void HitObject::write(std::ofstream& writer) const { std::visit([&writer](const auto& hit_object) { hit_object.write(writer); }, *this); }

//! HitObjects
void HitObjects::read(const std::vector<std::string>& contents)
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
			Floor floor(content);
			const auto time = floor.time;
			this->emplace_hint(back_itr, time, floor);

			break;
		}
		case static_cast<int32_t>(Template::Game::HitObject::HitObjectType::SLIDER):
		{
			if (content.size() < Slider::MINIMUM_NUM_CONTENT)
			{
				LOG_WARNNING(FormatExceptions::HitObjects::Format_HitObjects_NotEnoughtContent(line));
				continue;
			}
			Slider slider(content);
			const auto time = slider.time;
			this->emplace_hint(back_itr, time, slider);
			break;
		}
		default:
			LOG_WARNNING(FormatExceptions::HitObjects::Format_HitObjects_InvalidContent(line));
			break;
		}
	}
}
void HitObjects::write(std::ofstream& writer) const
{
	writer << FileFormat::Beatmap::HitObjects::HEADER << '\n';
	for (const auto& hit_object : *this | std::views::values)
		hit_object.write(writer);
	writer << '\n';
}