#pragma once
#include <filesystem>

namespace Work::Convert::osu
{
	void convert_mapset(const std::filesystem::path& file, const std::filesystem::path& output);
}