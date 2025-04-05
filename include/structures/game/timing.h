#pragma once
#include <cstdint>
#include <map>
#include "hitsound.h"

namespace GameObjects::Timing
{
	struct TimingPoint
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		int32_t time = 0;
		float beat_length;
		Hitsound::SampleSetType sample_set = Hitsound::SampleSetType::NO_CUSTOM;
		int32_t sample_index = 0;
		int32_t volume = 100;
		bool inherited = false;
		bool kiai = false;
		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;

		float get_velocity() const;

		TimingPoint() = default;
		TimingPoint(const std::vector<std::string>& content) { read(content); }
	};

	struct TimingPoints : std::multimap<int32_t, TimingPoint>
	{
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		TimingPoints() = default;
		explicit TimingPoints(const std::vector<std::string>& contents) { read(contents); }
	};
}