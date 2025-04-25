#include "structures/game/beatmap/hitobject.h" // Header
#include <fstream>
#include <ranges>
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "format/file.h"
#include "utilities.hpp"

namespace Structures::Game::Beatmap::HitObjects
{
	using Format::File::Floor::Mapset::AND;

	Types::Game::Direction::Direction get_next_direction(const Types::Game::Direction::Direction& prev_direction, const uint8_t& rotation)
	{
		return static_cast<Types::Game::Direction::Direction>((static_cast<uint8_t>(prev_direction) + rotation) % Types::Game::Direction::NUM_DIRECTIONS);
	}

	//! Floor
	void Floor::read(const std::vector<std::string>& content)
	{
		time = std::stoi(content[0]);
		rotation = static_cast<Types::Game::Direction::Rotation>(std::stoi(content[1]));
		combo_colour = static_cast<uint8_t>(std::stoi(content[2]));
		hit_sound = Hitsound::HitSound{ std::stoi(content[4]) };
		hit_sample = Hitsound::HitSample{ content[5] };
	}
	void Floor::write(std::ofstream& writer) const
	{
		writer << time << AND << static_cast<int32_t>(rotation) << AND << static_cast<int32_t>(combo_colour) << AND <<
			static_cast<bool>(type);
		writer << AND << hit_sound.to_int() << AND << hit_sample.to_string();
		writer << '\n';
	}
	// ::ActionInfo
	Floor::ActionInfo::ActionInfo(const Floor& floor, const Types::Game::Direction::Direction& previous_direction) :
		type(floor.type), direction(previous_direction + floor.rotation), time(floor.time)
	{
	}

	//! Slider
	void Slider::read(const std::vector<std::string>& content)
	{
		time = std::stoi(content[0]);
		rotation = static_cast<Types::Game::Direction::Rotation>(std::stoi(content[1]));
		combo_colour = static_cast<uint8_t>(std::stoi(content[2]));
		end_time = std::stoi(content[4]);

		// Curves
		/*
		for (const auto curves_str = Utilities::String::split(content[5], Format::File::Floor::Mapset::HitObjects::Slider::AND);
			 const auto& curves : curves_str)
		{
			const auto curve_str = Utilities::String::split(curves, Format::File::Floor::Mapset::HitObjects::Slider::CURVE_AND, true);
			const SliderCurve curve = {
				.after = std::stoi(curve_str[0]),
				.rotation = static_cast<Structures::Types::Game::Direction::Rotation>(std::stoi(curve_str[1]))
			};
			this->curves.push_back(curve);
		}
		*/
		hit_sound = Hitsound::HitSound{ std::stoi(content[5]) };
		hit_sample = Hitsound::HitSample{ content[6] };
	}
	void Slider::write(std::ofstream& writer) const
	{
		writer << time << AND << static_cast<int32_t>(rotation) << AND << static_cast<int32_t>(combo_colour) << AND <<
			static_cast<bool>(type) << AND << end_time;
		/*
		writer << AND;
		for (auto ptr = curves.begin(); ptr != curves.end(); ++ptr) {
			if (ptr != curves.begin()) writer << Format::File::Floor::Mapset::HitObjects::Slider::AND;
			writer << ptr->after << Format::File::Floor::Mapset::HitObjects::Slider::CURVE_AND << static_cast<int32_t>(ptr->rotation);
		}
		*/
		writer << AND << hit_sound.to_int() << AND << hit_sample.to_string();
		writer << '\n';
	}
	//:: ActionInfo
	Slider::ActionInfo::ActionInfo(const Slider& slider, const float& timing_velocity, const float& diff_velocity,
		const float& beat_length, const Types::Game::Direction::Direction& previous_direction) : type(slider.type), direction(previous_direction + slider.rotation),
		time(slider.time), end_time(slider.end_time)
	{
		const auto time_length = static_cast<float>(end_time - time);
		const auto speed = timing_velocity * diff_velocity;
		tick_length = time_length * speed;

		if (const auto tick_spacing_num = tick_length / beat_length;
			Utilities::Math::is_integer(tick_spacing_num))
			tick_num = static_cast<unsigned long>(std::max(0.0f, tick_spacing_num - 1));
		else tick_num = static_cast<unsigned long>(std::floor(tick_spacing_num));
		tick_num += 2; // 2 = Đầu Slider + Cuối Slider
	}

	//! HitObject
	int64_t HitObject::get_time() const { return std::visit([](const auto& hit_object) { return hit_object.time; }, *this); }
	int64_t HitObject::get_end_time() const
	{
		if (std::holds_alternative<Floor>(*this))
			return this->get_time();
		return std::get_if<Slider>(this)->end_time;
	}
	Types::Game::HitObject::HitObjectType HitObject::get_type() const { return std::visit([](const auto& hit_object) { return hit_object.type; }, *this); }
	Types::Game::Direction::Rotation HitObject::get_rotation() const { return std::visit([](const auto& hit_object) { return hit_object.rotation; }, *this); }
	uint8_t HitObject::get_combo_colour() const { return std::visit([](const auto& hit_object) { return hit_object.combo_colour; }, *this); }
	Hitsound::HitSound HitObject::get_hitsound() const { return std::visit([](const auto& hit_object) { return hit_object.hit_sound; }, *this); }
	Hitsound::HitSample HitObject::get_hitsample() const { return std::visit([](const auto& hit_object) { return hit_object.hit_sample; }, *this); }
	void HitObject::write(std::ofstream& writer) const { std::visit([&writer](const auto& hit_object) { hit_object.write(writer); }, *this); }

	//! HitObjects
	static std::pair<std::queue<Floor*>, std::queue<Slider*>> split_hit_objects_to_non_const_queue(HitObjects& hit_objects)
	{
		std::queue<Floor*> floor;
		std::queue<Slider*> slider;
		for (auto& hit_object : hit_objects | std::views::values)
		{
			if (std::holds_alternative<Floor>(hit_object))
				floor.push(std::get_if<Floor>(&hit_object));
			else if (std::holds_alternative<Slider>(hit_object))
				slider.push(std::get_if<Slider>(&hit_object));
		}
		return { floor, slider };
	}
	void HitObjects::read(const std::vector<std::string>& contents)
	{
		for (const auto& line : contents)
		{
			const auto content = Utilities::String::split(line, AND);
			if (content.size() < HitObject::MINIMUM_NUM_CONTENT)
			{
				LOG_WARNING(Logging::Exceptions::FormatExceptions::HitObjects::Format_HitObjects_NotEnoughContent(line));
				continue;
			}

			const auto back_itr = Utilities::Container::get_last_element_iterator(*this);
			switch (std::stoi(content[3]))
			{
			case static_cast<int32_t>(Types::Game::HitObject::HitObjectType::FLOOR):
			{
				Floor floor(content);
				const auto time = floor.time;
				this->emplace_hint(back_itr, time, floor);

				break;
			}
			case static_cast<int32_t>(Types::Game::HitObject::HitObjectType::SLIDER):
			{
				if (content.size() < Slider::MINIMUM_NUM_CONTENT)
				{
					LOG_WARNING(Logging::Exceptions::FormatExceptions::HitObjects::Format_HitObjects_NotEnoughContent(line));
					continue;
				}
				Slider slider(content);
				const auto time = slider.time;
				this->emplace_hint(back_itr, time, slider);
				break;
			}
			default:
				LOG_WARNING(Logging::Exceptions::FormatExceptions::HitObjects::Format_HitObjects_InvalidContent(line));
				break;
			}
		}

		//TODO: Kiểm tra nếu Floor/Slider nằm trên/chứa trong Slider khác thì những Floor/Slider đó không được xoay
	}
	std::pair<std::queue<const Floor*>, std::queue<const Slider*>> HitObjects::split_to_queue() const
	{
		std::queue<const Floor*> floor;
		std::queue<const Slider*> slider;
		for (const auto& hit_object : *this | std::views::values)
		{
			if (std::holds_alternative<Floor>(hit_object))
				floor.push(std::get_if<Floor>(&hit_object));
			else if (std::holds_alternative<Slider>(hit_object))
				slider.push(std::get_if<Slider>(&hit_object));
		}
		return { floor, slider };
	}
	void HitObjects::write(std::ofstream& writer) const
	{
		writer << Format::File::Floor::Mapset::HitObjects::HEADER << '\n';
		for (const auto& hit_object : *this | std::views::values)
			hit_object.write(writer);
		writer << '\n';
	}
}