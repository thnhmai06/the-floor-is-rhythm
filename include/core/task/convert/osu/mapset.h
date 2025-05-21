#pragma once
#include <filesystem>

namespace Core::Task::Convert::Osu
{
	void convert_mapset(std::filesystem::path path, const std::filesystem::path& output);
}