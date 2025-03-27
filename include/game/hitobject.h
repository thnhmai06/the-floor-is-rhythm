#pragma once
#include <cstdint>
#include <map>
#include <variant>
#include <vector>
#include "game/hitsound.h"
#include "logging.h"

enum class Direction : uint8_t
{
	RIGHT = 0,
	UP = 1,
	DOWN = 2,
	LEFT = 3
};

namespace HitObject
{
	constexpr uint8_t NUM_COMBOS = 8;
	constexpr uint8_t NUM_DIRECTIONS = 4;
	enum class HitObjectType : bool
	{
		FLOOR = 0,
		SLIDER = 1
	};

	inline Direction get_next_direction(Direction prev_direction, uint8_t direction_jump);

	struct Floor
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		int32_t time = 0;
		uint8_t direction_jump = 0;
		uint8_t combo_jump = 0;
		Hitsound::Hitsound hitsound;
		Hitsound::HitSample hitsample;

		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;
	};
	struct Slider
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 8;

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
		Hitsound::Hitsound hitsound;
		Hitsound::HitSample hitsample;

		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;
	};
	//Note: HitObject có thể chứa 2 kiểu, chứ không phải là cơ sở để tạo Floor và Slider
	struct HitObject : std::variant<Floor, Slider>
	{
	protected:
		using BASE = std::variant<Floor, Slider>;

	public:
		static constexpr int32_t MINIMUM_NUM_CONTENT = (Floor::MINIMUM_NUM_CONTENT < Slider::MINIMUM_NUM_CONTENT)
			                                               ? Floor::MINIMUM_NUM_CONTENT
			                                               : Slider::MINIMUM_NUM_CONTENT;

		void read(const std::string& line);
		HitObject() = default;
		HitObject(const std::string& line) { read(line); }
		HitObject(const Floor& floor) : BASE(floor) {}
		HitObject(const Slider& slider) : BASE(slider) {}
	};
	struct HitObjects : std::multimap<int32_t, HitObject>
	{
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;
	};
}