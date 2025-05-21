#pragma once
#include <map>
#include "structures/config.h"
#include "structures/game/mapset/hitsound.h"

namespace Structures::Game::Beatmap::TimingPoints
{
	struct TimingPoint : Config::Section
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 6;

		int64_t time = 0;
		float beat_length;
		Hitsound::TimingSample sample;
		bool inherited = false;
		bool kiai = false;

		void read(const std::vector<std::string>& content) override;
		[[nodiscard]] std::string to_string() const override;
		[[nodiscard]] float get_velocity() const;

		TimingPoint() = default;
		explicit TimingPoint(const std::vector<std::string>& content);
	};

	struct TimingPoints : Config::Section
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
		using Container = std::multimap<int64_t, TimingPoint>;

		Container data;

		void read(const std::vector<std::string>& contents) override;
		[[nodiscard]] std::string to_string() const override;

		TimingPoints() = default;
		explicit TimingPoints(const std::vector<std::string>& contents);
	};
}