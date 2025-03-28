#pragma once
#include <cstdint>
#include <SDL3/SDL_audio.h>
#include "audio/buses.h"
#include "template.h"

using namespace Template::Audio;

struct Mixer {
	int32_t volume;
	SDL_AudioSpec spec;
	AudioBus<Music> music;
	AudioBus<Effect> effect;

	int32_t set_master_volume(int32_t value = -1);
	void quit();
	Mixer();
};