#pragma once
#include "game/metadata.h"
#include "game/hitobject.h"
#include "game/timing.h"

using namespace GameObjects::Metadata;
using namespace GameObjects::HitObjects;
using namespace GameObjects::Timing;

struct BeatmapFile
{
	General general;
	Metadata metadata;
	CalculatedDifficulty calculated_difficulty;
	HitObjects hit_objects;
	TimingPoints uninherited_points;
	TimingPoints inherited_points;

	BeatmapFile(const char* file_path);
};