#pragma once
#include <memory>
#include "structures/audio/mixer.h"
#include "config.h"

namespace Work::Audio
{
	using namespace Structures::Audio;
	using namespace Config::Game::Audio;
	using Config::user_config;

	namespace Memory
	{
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

	inline std::unique_ptr<Mixer> mixer;

	inline void init()
	{
		SDL_AudioSpec spec;
		spec.channels = CHANNELS_STEREO;
		spec.format = AUDIO_FORMAT;
		spec.freq = SAMPLE_FREQUENCY;

		mixer = std::make_unique<Mixer>(spec, user_config->audio.volume, MAX_CHANNELS);
	}
	inline void quit()
	{
		if (mixer)
		{
			Mixer::quit();
			mixer.reset();
		}
	}
}