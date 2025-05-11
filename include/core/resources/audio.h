#pragma once
#include <memory>
#include "structures/audio/mixer.h"
#include "config.h"

namespace Core::Resources
{
	namespace Audio
	{
		using namespace Structures::Audio;
		using namespace Config::Game::Audio;
		using Config::user_config;

		namespace Beatmap
		{
			inline MusicMemory music;
			inline EffectMemory effect;
		}
		namespace Skin
		{
			inline MusicMemory music;
			inline EffectMemory effect;
		}
	}
}