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
		void write(std::ofstream& writer) const;

		[[nodiscard]] int64_t get_time() const;
		[[nodiscard]] float get_velocity() const;

		TimingPoint() = default;
		explicit TimingPoint(const std::vector<std::string>& content) { read(content); }
	};

	struct TimingPoints : std::multimap<int32_t, TimingPoint>
	{
		// [inherited, uninherited]
		[[nodiscard]] std::pair<std::queue<const TimingPoint*>, std::queue<const TimingPoint*>> split_to_queue() const; 

		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		TimingPoints() = default;
		explicit TimingPoints(const std::vector<std::string>& contents) { read(contents); }
	};
}