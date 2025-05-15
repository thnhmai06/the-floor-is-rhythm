#pragma once
#include "config.h"

namespace Core::Work::Convert::Osu
{
	void convert_beatmap(fs::path path, const std::filesystem::path& output = Config::Game::General::Path::BEATMAP);
}