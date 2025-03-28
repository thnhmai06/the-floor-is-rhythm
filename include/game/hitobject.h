#pragma once
#include <cstdint>
#include <map>
#include <vector>
#include "game/hitsound.h"

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
	enum class DirectionJump : uint8_t
	{
		NO_ROTATE = 0,
		ROTATE_90 = 1,
		ROTATE_180 = 2,
		ROTATE_270 = 3
	};

	inline Direction get_next_direction(Direction prev_direction, uint8_t direction_jump);

	struct HitObject
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		int32_t time = 0;
		int32_t end_time = 0;
		HitObjectType type;
		DirectionJump direction_jump = DirectionJump::NO_ROTATE;
		uint8_t combo_jump = 0;
		Hitsound::Hitsound hitsound;
		Hitsound::HitSample hitsample;

		virtual void read(const std::vector<std::string>& content) = NULL;
		virtual void write(std::ofstream& writer) const = NULL;
		virtual ~HitObject() = default;
	};
	struct Floor : HitObject
	{
		void read(const std::vector<std::string>& content) override;
		void write(std::ofstream& writer) const override;

		Floor() { type = HitObjectType::FLOOR; }
		explicit Floor(const std::vector<std::string>& content): Floor() { Floor::read(content); }
	};
	struct Slider : HitObject
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 8;

		struct SliderCurve
		{
			int32_t padding_time = 0; // CurveTime = time + padding_time
			DirectionJump direction_jump = DirectionJump::NO_ROTATE;
		};
		std::vector<SliderCurve> curves;

		void read(const std::vector<std::string>& content) override;
		void write(std::ofstream& writer) const override;
		Slider() { type = HitObjectType::SLIDER; }
		explicit Slider(const std::vector<std::string>& content): Slider() { Slider::read(content); }
	};

	struct HitObjects : std::multimap<int32_t, std::unique_ptr<HitObject>>
	{
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		HitObjects() = default;
		HitObjects(const std::vector<std::string>& contents) { read(contents); }
	};
}