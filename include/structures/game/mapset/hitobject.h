#pragma once
#include <map>
#include <vector>
#include "structures/type.hpp"
#include "structures/game/mapset/hitsound.h"
#include "utilities.hpp"

namespace Structures::Game::Beatmap::HitObjects
{
	struct Floor
	{
		static constexpr int32_t MINIMUM_NUM_CONTENT = 4;

		float time = 0;
		bool is_kat;
		Hitsound::HitSound hit_sound{};
		Hitsound::HitSample hit_sample{};

		void read(const std::vector<std::string>& contents);
		[[nodiscard]] std::string to_string() const;

		Floor() = default;
		explicit Floor(const std::vector<std::string>& content);
	};

	struct HitObjects
	{
		using CONTAINER = std::map<float, Floor>;

		CONTAINER data;

		void read(const std::vector<std::string>& lines);
		[[nodiscard]] std::string to_string() const;

		HitObjects() = default;
		explicit HitObjects(const std::vector<std::string>& contents) { read(contents); }
		friend std::ostream& operator<<(std::ostream& os, const HitObjects& hit_objects);
	};
}