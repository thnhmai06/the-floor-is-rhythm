#pragma once
#include "game/object.h"
#include "game/metadata.h"

struct BeatmapFile
{
	Metadata::General general;
	Metadata::Metadata metadata;
	Metadata::CalculatedDifficulty calculated_difficulty;
	HitObjects hit_objects;

	BeatmapFile(const char* file_path);
};