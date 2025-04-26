#pragma once
#include <cstdint>
#include <map>
#include <queue>
#include "structures/game/beatmap/hitsound.h"

namespace Structures::Game::Beatmap::TimingPoints
{
	struct TimingPoint
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		int64_t time = 0;
		float beat_length;
		Hitsound::HitSampleType sample_set = Hitsound::HitSampleType::NO_CUSTOM;
		int32_t sample_index = 0;
		int32_t volume = 100;
		bool inherited = false;
		bool kiai = false;

		void read(const std::vector<std::string>& content);
		std::string to_string() const;

		[[nodiscard]] int64_t get_time() const;
		[[nodiscard]] float get_velocity() const;

		TimingPoint() = default;
		explicit TimingPoint(const std::vector<std::string>& content) { read(content); }
		friend std::ostream& operator<<(std::ostream& os, const TimingPoint& point);
	};

	struct TimingPoints
	{
		std::multimap<int32_t, TimingPoint> data;

		// [inherited, uninherited]
		[[nodiscard]] std::pair<std::queue<const TimingPoint*>, std::queue<const TimingPoint*>> split_to_queue() const;
		[[nodiscard]] std::pair<std::multimap<int64_t, const TimingPoint*>, std::multimap<int64_t, const TimingPoint*>> split_to_map() const;
		std::queue<const TimingPoint*> to_queue() const;

		void read(const std::vector<std::string>& contents);
		std::string to_string() const;
		friend std::ostream& operator<<(std::ostream& os, const TimingPoints& points);

		TimingPoints() = default;
		explicit TimingPoints(const std::vector<std::string>& contents) { read(contents); }
	};
}