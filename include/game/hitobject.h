#pragma once
#include <cstdint>
#include <map>
#include <vector>
#include "template.h"
#include "game/hitsound.h"

namespace HitObject
{
	constexpr uint8_t NUM_COMBOS = 8;

	inline Template::Game::Direction::Direction get_next_direction(Template::Game::Direction::Direction prev_direction, uint8_t rotation);

	struct HitObject
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		int32_t time = 0;
		int32_t end_time = 0;
		Template::Game::HitObject::HitObjectType type;
		Template::Game::Direction::Rotation rotation = Template::Game::Direction::Rotation::NO_ROTATE;
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

		Floor() { type = Template::Game::HitObject::HitObjectType::FLOOR; }
		explicit Floor(const std::vector<std::string>& content): Floor() { Floor::read(content); }
	};
	struct Slider : HitObject
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 8;

		struct SliderCurve
		{
			uint32_t after = 0;
			Template::Game::Direction::Rotation rotation = Template::Game::Direction::Rotation::NO_ROTATE;
		};
		std::vector<SliderCurve> curves;

		void read(const std::vector<std::string>& content) override;
		void write(std::ofstream& writer) const override;
		Slider() { type = Template::Game::HitObject::HitObjectType::SLIDER; }
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