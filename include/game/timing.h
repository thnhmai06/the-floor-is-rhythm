#pragma once
#include <cstdint>
#include <variant>
#include <map>
#include "hitsound.h"

namespace Timing
{
	struct UninheritedPoint
	{
		int32_t time;
		double beat_length;
		int32_t meter;
		Hitsound::SampleSetType sample_set = Hitsound::SampleSetType::NO_CUSTOM;
		int32_t sample_index = 0;
		int32_t volume = 100;
		bool kiai = false;

		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;
	};
	struct InheritedPoint
	{
		int32_t time;
		double velocity = 1;
		int32_t meter;
		Hitsound::SampleSetType sample_set = Hitsound::SampleSetType::NO_CUSTOM;
		int32_t sample_index = 0;
		int32_t volume = 100;
		bool kiai = false;

		void read(const std::vector<std::string>& content);
		void write(std::ofstream& writer) const;
	};

	// TimingPoint Tương tự như HitObject - Floor, Slider
	struct TimingPoint: std::variant<UninheritedPoint, InheritedPoint>
	{
	protected:
		using BASE = std::variant<UninheritedPoint, InheritedPoint>;

	public:
		static constexpr int32_t MINIMUM_NUM_CONTENT = 7;

		void read(const std::string& line);
		void write(std::ofstream& writer) const {
			std::visit([&writer](const auto& point) {point.write(writer); }, *this);
		}

		TimingPoint() = default;
		TimingPoint(const std::string& line) { read(line); }
		TimingPoint(const UninheritedPoint& uninherited_point) : BASE(uninherited_point) {}
		TimingPoint(const InheritedPoint& inherited_point) : BASE(inherited_point) {}
	};
	struct TimingPoints : std::multimap<int32_t, TimingPoint>
	{
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;
	};
}