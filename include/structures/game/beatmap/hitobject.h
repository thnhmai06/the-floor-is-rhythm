#pragma once
#include <map>
#include <vector>
#include "structures/types.h"
#include "structures/game/beatmap/hitsound.h"
#include "utilities.hpp"

namespace Structures::Game::Beatmap::HitObjects
{
	constexpr uint8_t COLOURS_NUM = 8;
	// type_dat lấy 4 bit - bit 1 là kiểu (1 - Slider, 0 - Floor), bit 2 + 3 + 4 biểu diễn colour
	struct TypeData final
	{
		Types::Game::HitObject::HitObjectType type;
		uint8_t combo_colour;

		void read(const uint8_t& data);
		void read(const int32_t& data);
		uint8_t to_int() const;
		TypeData() = default;
		explicit TypeData(const uint8_t& data) { read(data); }
		explicit TypeData(const int32_t& data) { read(data); }
	};

	struct Floor
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 4;

		int64_t time = 0;
		int64_t end_time = 0;
		TypeData type_data;
		Hitsound::HitSound hit_sound;
		Hitsound::HitSample hit_sample;

		virtual void read(const std::vector<std::string>& content);
		[[nodiscard]] virtual std::string to_string() const;

		Floor();
		explicit Floor(const std::vector<std::string>& content);
		virtual ~Floor() = default;
	};
	struct Slider final : Floor
	{
		struct Sample
		{
			Hitsound::HitSampleType normal_set = Hitsound::HitSampleType::NO_CUSTOM;
			Hitsound::HitSampleType addition_set = Hitsound::HitSampleType::NO_CUSTOM;

			Sample(const OsuParser::Beatmap::Objects::HitObject::HitObject::SliderParams::EdgeHitsound::SampleSet& osu_slider_sample)
				: normal_set(static_cast<Hitsound::HitSampleType>(static_cast<std::int32_t>(osu_slider_sample.NormalSet))),
				addition_set(static_cast<Hitsound::HitSampleType>(static_cast<std::int32_t>(osu_slider_sample.AdditionSet)))
			{
			}

			std::string to_string() const;
		};

		static constexpr int32_t MINIMUM_NUM_CONTENT = 8;
		uint32_t slides = 1;
		std::vector<Hitsound::HitSound> edge_sounds;
		std::vector<Sample> edge_sets;

		void read(const std::vector<std::string>& content) override;
		[[nodiscard]] std::string to_string() const override;

		Slider();
		explicit Slider(const std::vector<std::string>& content);
	};

	struct HitObjects
	{
		std::multimap<int32_t, std::shared_ptr<Floor>> data;

		std::queue<std::weak_ptr<const Floor>> make_queue() const;
		void read(const std::vector<std::string>& lines);
		std::string to_string() const;

		HitObjects() = default;
		explicit HitObjects(const std::vector<std::string>& contents) { read(contents); }
		friend std::ostream& operator<<(std::ostream& os, const HitObjects& hit_objects);
	};
}