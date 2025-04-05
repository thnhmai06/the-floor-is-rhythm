#pragma once
#include <cstdint>
#include <map>
#include <variant>
#include <vector>
#include "template.h"
#include "structures/game/hitsound.h"

namespace GameObjects::HitObjects
{
	constexpr uint8_t NUM_COMBOS = 8;
	inline Template::Game::Direction::Direction get_next_direction(const Template::Game::Direction::Direction& prev_direction, uint8_t rotation);

	struct Floor
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		int32_t time = 0;
		int32_t end_time = 0;
		Template::Game::HitObject::HitObjectType type = Template::Game::HitObject::HitObjectType::FLOOR;
		Template::Game::Direction::Rotation rotation = Template::Game::Direction::Rotation::NO_ROTATE;
		uint8_t combo_jump = 0;
		Hitsound::Hitsound hit_sound;
		Hitsound::HitSample hit_sample;

		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;

		Floor() { type = Template::Game::HitObject::HitObjectType::FLOOR; }
		explicit Floor(const std::vector<std::string>& content): Floor() { read(content); }
	};
	struct Slider
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 8;
		struct SliderCurve
		{
			int32_t after = 0;
			Template::Game::Direction::Rotation rotation = Template::Game::Direction::Rotation::NO_ROTATE;
		};

		int32_t time = 0;
		int32_t end_time = 0;
		Template::Game::HitObject::HitObjectType type = Template::Game::HitObject::HitObjectType::FLOOR;
		Template::Game::Direction::Rotation rotation = Template::Game::Direction::Rotation::NO_ROTATE;
		uint8_t combo_jump = 0;
		std::vector<SliderCurve> curves;
		Hitsound::Hitsound hit_sound;
		Hitsound::HitSample hit_sample;

		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;

		Slider() { type = Template::Game::HitObject::HitObjectType::SLIDER; }
		explicit Slider(const std::vector<std::string>& content): Slider() { read(content); }
	};
	struct HitObject : std::variant<Floor, Slider>
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		void write(std::ofstream& writer) const;
		[[nodiscard]] int32_t get_time() const;
		[[nodiscard]] int32_t get_end_time() const;
		[[nodiscard]] Template::Game::HitObject::HitObjectType get_type() const;
		[[nodiscard]] Template::Game::Direction::Rotation get_rotation() const;
		[[nodiscard]] uint8_t get_combo_jump() const;
		[[nodiscard]] Hitsound::Hitsound get_hitsound() const;
		[[nodiscard]] Hitsound::HitSample get_hitsample() const;
	};

	struct HitObjects : std::multimap<int32_t, HitObject>
	{
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		HitObjects() = default;
		HitObjects(const std::vector<std::string>& contents) { read(contents); }
	};
}