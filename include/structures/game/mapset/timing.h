#pragma once
#include <map>
#include "structures/game/mapset/hitsound.h"

namespace Structures::Game::Beatmap::TimingPoints
{
	struct TimingPoint
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		int64_t time = 0;
		float beat_length;
		Hitsound::TimingSample sample;
		bool inherited = false;
		bool kiai = false;

		void read(const std::vector<std::string>& content);
		[[nodiscard]] std::string to_string() const;

		[[nodiscard]] float get_velocity() const;

		TimingPoint() = default;
		explicit TimingPoint(const std::vector<std::string>& content) { read(content); }
		friend std::ostream& operator<<(std::ostream& os, const TimingPoint& point);
	};

	struct TimingPoints
	{
	protected:
		struct TimingPos
		{
			float pos;
			float beat_length;
			float velocity;
		};
		std::multimap<int64_t, TimingPos> timing_pos;

	public:
		using CONTAINER = std::multimap<int64_t, TimingPoint>;

		CONTAINER data;

		[[nodiscard]] float get_object_pos(const float& current_time) const;
		void read(const std::vector<std::string>& contents);
		[[nodiscard]] std::string to_string() const;
		friend std::ostream& operator<<(std::ostream& os, const TimingPoints& points);

		TimingPoints() = default;
		explicit TimingPoints(const std::vector<std::string>& contents);
	};
}