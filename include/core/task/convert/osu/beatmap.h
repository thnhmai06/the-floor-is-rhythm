#pragma once
#include "config.h"

namespace Core::Task::Convert::Osu
{
	void convert_beatmap(fs::path path, const std::filesystem::path& output = Config::Game::General::Path::BEATMAP);
}