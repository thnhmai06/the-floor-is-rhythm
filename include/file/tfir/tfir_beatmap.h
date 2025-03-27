#pragma once
#include "game/metadata.h"
#include "game/hitobject.h"
#include "game/timing.h"

struct BeatmapFile
{
	Metadata::General general;
	Metadata::Metadata metadata;
	Metadata::CalculatedDifficulty calculated_difficulty;
	HitObject::HitObjects hit_objects;
	Timing::TimingPoints timing_points;

	BeatmapFile(const char* file_path);
};