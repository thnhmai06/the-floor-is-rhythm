#pragma once
#include <memory>
#include "structures/audio/mixer.h"
#include "config.h"

namespace Work::Audio
{
	using namespace  Structures::Audio;
	using namespace Config::GameConfig::Audio;
	using namespace Config::UserConfig::Audio;

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

		mixer = std::make_unique<Mixer>(spec,
			Volume::master, Volume::music, Volume::effects, MAX_CHANNELS);
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