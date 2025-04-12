#pragma once
#include "structures/game/beatmap/metadata.h"
#include "structures/game/beatmap/hitobject.h"
#include "structures/game/beatmap/timing.h"

namespace Structures::Game::Beatmap
{
	struct Beatmap
	{
		Metadata::General general;
		Metadata::Metadata metadata;
		Metadata::CalculatedDifficulty calculated_difficulty;
		HitObjects::HitObjects hit_objects;
		TimingPoints::TimingPoints timing_points;

		unsigned long total_floor = 0;
		unsigned long total_slider = 0;
		unsigned long total_objects_num = 0;
		unsigned long total_combo = 0;

		explicit Beatmap(const char* file_path);
	};
}