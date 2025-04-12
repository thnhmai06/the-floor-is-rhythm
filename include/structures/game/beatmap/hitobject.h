#pragma once
#include <cstdint>
#include <map>
#include <variant>
#include <vector>
#include "structures/types.h"
#include "structures/game/beatmap/hitsound.h"

namespace Structures::Game::Beatmap::HitObjects
{
	constexpr uint8_t NUM_COMBOS = 8;
	inline Types::Game::Direction::Direction get_next_direction(const Types::Game::Direction::Direction& prev_direction, uint8_t rotation);

	struct Floor
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		int32_t time = 0;
		int32_t end_time = 0;
		Types::Game::HitObject::HitObjectType type = Types::Game::HitObject::HitObjectType::FLOOR;
		Types::Game::Direction::Rotation rotation = Types::Game::Direction::Rotation::NO_ROTATE;
		uint8_t combo_jump = 0;
		Hitsound::Hitsound hit_sound;
		Hitsound::HitSample hit_sample;

		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;

		Floor() { type = Types::Game::HitObject::HitObjectType::FLOOR; }
		explicit Floor(const std::vector<std::string>& content): Floor() { read(content); }
	};
	struct Slider
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 7;
		/*struct SliderCurve
		{
			int32_t after = 0;
			Types::Logic::Direction::Rotation rotation = Types::Logic::Direction::Rotation::NO_ROTATE;
		};*/

		int32_t time = 0;
		int32_t end_time = 0;
		Types::Game::HitObject::HitObjectType type = Types::Game::HitObject::HitObjectType::FLOOR;
		Types::Game::Direction::Rotation rotation = Types::Game::Direction::Rotation::NO_ROTATE;
		uint8_t combo_jump = 0;
		//std::vector<SliderCurve> curves;
		Hitsound::Hitsound hit_sound;
		Hitsound::HitSample hit_sample;

		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;

		Slider() { type = Types::Game::HitObject::HitObjectType::SLIDER; }
		explicit Slider(const std::vector<std::string>& content): Slider() { read(content); }
	};
	struct HitObject : std::variant<Floor, Slider>
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		void write(std::ofstream& writer) const;
		[[nodiscard]] int32_t get_time() const;
		[[nodiscard]] int32_t get_end_time() const;
		[[nodiscard]] Types::Game::HitObject::HitObjectType get_type() const;
		[[nodiscard]] Types::Game::Direction::Rotation get_rotation() const;
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