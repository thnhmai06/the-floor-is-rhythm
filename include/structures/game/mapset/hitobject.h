#pragma once
#include <map>
#include <vector>
#include "structures/config.h"
#include "structures/game/mapset/hitsound.h"

namespace Structures::Game::Beatmap::HitObjects
{
	struct Floor : Config::Section
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 4;

		float time = 0;
		bool is_kat;
		Hitsound::Additions hit_sound{};
		Hitsound::HitSample hit_sample{};

		void read(const std::vector<std::string>& contents) override;
		[[nodiscard]] std::string to_string() const override;

		Floor() = default;
		explicit Floor(const std::vector<std::string>& contents);
	};

	struct HitObjects : Config::Section
	{
		using CONTAINER = std::map<float, Floor>;

		CONTAINER data;

		void read(const std::vector<std::string>& lines) override;
		[[nodiscard]] std::string to_string() const override;

		HitObjects() = default;
		explicit HitObjects(const std::vector<std::string>& lines);
	};
}