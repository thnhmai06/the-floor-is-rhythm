#pragma once
#include <filesystem>

namespace Core::Work::Convert::Osu
{
	void convert_mapset(std::filesystem::path path, const std::filesystem::path& output);
}