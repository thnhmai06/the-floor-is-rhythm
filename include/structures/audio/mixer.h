#pragma once
#include <cstdint>
#include <SDL3/SDL_audio.h>
#include "structures/audio/buses.h"
#include "structures/types.h"

namespace Structures::Audio::Mixer
{
	using namespace Structures::Audio::Buses;

	struct Mixer
	{
		int32_t volume;
		SDL_AudioSpec spec;
		AudioBus<Music> music;
		AudioBus<Effect> effect;

		int32_t set_master_volume(int32_t value = -1);
		void quit();
		Mixer();
	};
}