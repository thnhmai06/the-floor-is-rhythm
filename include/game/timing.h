#pragma once
#include <cstdint>
#include <map>
#include "hitsound.h"

namespace GameObjects::Timing
{
	struct TimingPoint
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 7;

		int32_t time = 0;
		Hitsound::SampleSetType sample_set = Hitsound::SampleSetType::NO_CUSTOM;
		int32_t sample_index = 0;
		int32_t volume = 100;
		bool kiai = false;

		virtual void read(const std::vector<std::string>& content) = NULL;
		virtual void write(std::ofstream& writer) const = NULL;
		virtual ~TimingPoint() = default;
	};
	struct UninheritedPoint : TimingPoint
	{
		float beat_length;
		void read(const std::vector<std::string>& content) override;
		void write(std::ofstream& writer) const override;

		UninheritedPoint() = default;
		UninheritedPoint(const std::vector<std::string>& content) { UninheritedPoint::read(content); }
	};
	struct InheritedPoint : TimingPoint
	{
		float velocity;
		void read(const std::vector<std::string>& content) override;
		void write(std::ofstream& writer) const override;

		InheritedPoint() = default;
		InheritedPoint(const std::vector<std::string>& content) { InheritedPoint::read(content); }
	};

	struct TimingPoints : std::multimap<int32_t, std::unique_ptr<TimingPoint>>
	{
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		TimingPoints() = default;
		TimingPoints(const std::vector<std::string>& contents) { read(contents); }
	};
}