#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <cstdint>
#include <fstream>
#include <vector>
#include "file_format.h"
#include "game/hitsound.h"

namespace HitObject
{
	using namespace tfir::HitObjects;
	using tfir::SEPARATOR, tfir::AND, tfir::SECTION_END;
	constexpr uint8_t NUM_COMBOS = 8;
	constexpr uint8_t NUM_DIRECTIONS = 4;

	enum class Direction : uint8_t
	{
		RIGHT = 0,
		UP = 1,
		DOWN = 2,
		LEFT = 3
	};
	inline Direction get_next_direction(const Direction prev_direction, const uint8_t direction_jump)
	{
		return static_cast<Direction>((static_cast<uint8_t>(prev_direction) + direction_jump) % NUM_DIRECTIONS);
	}

	struct Floor
	{
		int time = 0;
		uint8_t direction_jump = 0;
		uint8_t combo_jump = 0;
		Hitsound hitsound;
		HitSample hitsample;

		void write(std::ofstream& writer) const
		{
			writer << time << AND << static_cast<int32_t>(direction_jump) << AND << static_cast<int32_t>(combo_jump) << AND << Type::FLOOR;
			writer << AND << hitsound.ToInt() << AND << hitsample.ToString();
			writer << '\n';
		}
	};
	struct Slider
	{
		struct Curve
		{
			int add_time = 0; // CurveTime = time + add_time
			uint8_t direction_jump = 0;
		};

		int time = 0;
		int end_time = 0;
		uint8_t direction_jump = 0;
		std::vector<Curve> curves;
		uint8_t combo_jump = 0;
		Hitsound hitsound;
		HitSample hitsample;

		void write(std::ofstream& writer) const
		{
			writer << time << AND << static_cast<int32_t>(direction_jump) << AND << static_cast<int32_t>(combo_jump) << AND << Type::SLIDER << AND << end_time << AND;
			for (auto ptr = curves.begin(); ptr != curves.end(); ++ptr) {
				if (ptr != curves.begin()) writer << SLIDER_AND;
				writer << ptr->add_time << SLIDER_AND << static_cast<int32_t>(ptr->direction_jump);
			}
			writer << hitsound.ToInt() << AND << hitsample.ToString();
			writer << '\n';
		}
	};
};

#endif
