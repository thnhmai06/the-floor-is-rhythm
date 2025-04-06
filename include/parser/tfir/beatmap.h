#pragma once
#include "structures/game/metadata.h"
#include "structures/game/hitobject.h"
#include "structures/game/timing.h"

struct BeatmapFile
{
	GameObjects::Metadata::General general;
	GameObjects::Metadata::Metadata metadata;
	GameObjects::Metadata::CalculatedDifficulty calculated_difficulty;
	GameObjects::HitObjects::HitObjects hit_objects;
	GameObjects::Timing::TimingPoints timing_points;

	BeatmapFile(const char* file_path);
};