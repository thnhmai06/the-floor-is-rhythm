#pragma once
#include <cstdint>
#include <map>
#include <variant>
#include <vector>
#include <queue>
#include "structures/types.h"
#include "structures/game/beatmap/hitsound.h"
#include "utilities.hpp"

namespace Structures::Game::Beatmap::HitObjects
{
	constexpr uint8_t NUM_COMBOS = 8;
	inline Types::Game::Direction::Direction get_next_direction(const Types::Game::Direction::Direction& prev_direction, const uint8_t& rotation);

	struct Floor
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		int64_t time;
		const Types::Game::HitObject::HitObjectType type = Types::Game::HitObject::HitObjectType::FLOOR;
		Types::Game::Direction::Rotation rotation;
		uint8_t combo_colour;
		Hitsound::Hitsound hit_sound;
		Hitsound::HitSample hit_sample;

		struct Action
		{
			Types::Game::HitObject::HitObjectType type;
			Types::Game::Direction::Direction direction;
			int64_t time;

			explicit Action(const Floor& floor,
				const Types::Game::Direction::Direction& previous_direction =
				Types::Game::Direction::Direction::RIGHT);
		};

		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;

		Floor() = default;
		explicit Floor(const std::vector<std::string>& content) : Floor() { read(content); }
	};
	struct Slider
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 7;

		int64_t time;
		int64_t end_time;
		const Types::Game::HitObject::HitObjectType type = Types::Game::HitObject::HitObjectType::SLIDER;
		Types::Game::Direction::Rotation rotation;
		uint8_t combo_colour;
		Hitsound::Hitsound hit_sound;
		Hitsound::HitSample hit_sample;

		struct Action
		{
			Types::Game::HitObject::HitObjectType type;
			Types::Game::Direction::Direction direction;
			int64_t time, end_time;
			float tick_length = 0;
			unsigned long tick_num = 0;

			explicit Action(const Slider& slider, const float& timing_velocity, const float& diff_velocity, const float& beat_length,
				const Types::Game::Direction::Direction& previous_direction = Types::Game::Direction::Direction::RIGHT);
		};

		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;

		Slider() = default;
		explicit Slider(const std::vector<std::string>& content) { read(content); }
	};
	struct HitObject : std::variant<Floor, Slider>
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		void write(std::ofstream& writer) const;
		[[nodiscard]] int64_t get_time() const;
		[[nodiscard]] int64_t get_end_time() const;
		[[nodiscard]] Types::Game::HitObject::HitObjectType get_type() const;
		[[nodiscard]] Types::Game::Direction::Rotation get_rotation() const;
		[[nodiscard]] uint8_t get_combo_colour() const;
		[[nodiscard]] Hitsound::Hitsound get_hitsound() const;
		[[nodiscard]] Hitsound::HitSample get_hitsample() const;
	};

	struct HitObjects : std::multimap<int32_t, HitObject>
	{
		// [Floor, Slider]
		[[nodiscard]] std::pair<std::queue<const Floor*>, std::queue<const Slider*>> split_to_queue() const;

		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		HitObjects() = default;
		explicit HitObjects(const std::vector<std::string>& contents) { read(contents); }
	};
}