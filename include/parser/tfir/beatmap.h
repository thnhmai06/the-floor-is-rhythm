#pragma once
#include "structures/game/metadata.h"
#include "structures/game/hitobject.h"
#include "structures/game/timing.h"

using namespace GameObjects::Metadata;
using namespace GameObjects::HitObjects;
using namespace GameObjects::Timing;

struct BeatmapFile
{
	General general;
	Metadata metadata;
	CalculatedDifficulty calculated_difficulty;
	HitObjects hit_objects;
	TimingPoints timing_points;

	BeatmapFile(const char* file_path);
};