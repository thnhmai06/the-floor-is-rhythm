#pragma once
#include "structures/game/metadata.h"
#include "structures/game/hitobject.h"
#include "structures/game/timing.h"

struct Mapset
{
	GameObjects::Metadata::General general;
	GameObjects::Metadata::Metadata metadata;
	GameObjects::Metadata::CalculatedDifficulty calculated_difficulty;
	GameObjects::HitObjects::HitObjects hit_objects;
	GameObjects::Timing::TimingPoints timing_points;

	unsigned long total_floor = 0;
	unsigned long total_slider = 0;
	unsigned long total_objects_num = 0;
	unsigned long total_combo = 0;

	explicit Mapset(const char* file_path);
};