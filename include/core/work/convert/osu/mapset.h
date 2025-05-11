#pragma once
#include <filesystem>

namespace Core::Work::Convert::Osu
{
	void convert_mapset(const std::filesystem::path& file, const std::filesystem::path& output);
}