#pragma once
#include "work/convert/osu/mapset.h"

namespace Work::Convert::osu
{
	void convert_beatmap(const std::filesystem::path& path, const std::filesystem::path& output);
}