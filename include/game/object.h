#pragma once
#include <cstdint>
#include <vector>
#include "game/hitsound.h"

namespace HitObject
{
	constexpr uint8_t NUM_COMBOS = 8;
	constexpr uint8_t NUM_DIRECTIONS = 4;

	enum class Direction : uint8_t
	{
		RIGHT = 0,
		UP = 1,
		DOWN = 2,
		LEFT = 3
	};
	inline Direction get_next_direction(const Direction prev_direction, const uint8_t direction_jump);

	struct Floor
	{
		int32_t time = 0;
		uint8_t direction_jump = 0;
		uint8_t combo_jump = 0;
		Hitsound hitsound;
		HitSample hitsample;

		void write(std::ofstream& writer) const;
	};
	struct Slider
	{
		struct Curve
		{
			int32_t add_time = 0; // CurveTime = time + add_time
			uint8_t direction_jump = 0;
		};

		int32_t time = 0;
		int32_t end_time = 0;
		uint8_t direction_jump = 0;
		std::vector<Curve> curves;
		uint8_t combo_jump = 0;
		Hitsound hitsound;
		HitSample hitsample;

		void write(std::ofstream& writer) const;
	};
}