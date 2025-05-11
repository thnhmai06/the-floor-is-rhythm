#pragma once
#include "config.h"

namespace Core::Work::Convert::Osu
{
	void convert_beatmap(const std::filesystem::path& path, const std::filesystem::path& output = Config::Game::General::Path::BEATMAP);
}