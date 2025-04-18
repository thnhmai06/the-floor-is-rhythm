﻿#include "structures/audio/mixer.h" // Header
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "utilities.hpp"

namespace Structures::Audio
{
	float Mixer::get_volume() const { return volume; }
	float Mixer::set_volume(const float& percent)
	{
		if (percent < 0) return set_volume(0);
		if (percent > 1) return set_volume(1);
		volume = percent;

		return Utilities::Math::to_percent(
			Mix_MasterVolume(
				Utilities::Math::to_value(percent, 0, MIX_MAX_VOLUME)),
			0, MIX_MAX_VOLUME);
	}
	// ::Mixer
	void Mixer::quit() { Mix_Quit(); }
	Mixer::Mixer(const SDL_AudioSpec& spec, const float& master_volume, const float& music_volume,
		const float& effects_volume, const int32_t& max_channels) : spec(spec), music(music_volume), effect(effects_volume)
	{
		//Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

		//Mix_AllocateChannels(MAX_CHANNELS);
		//spec.channels = CHANNELS_STEREO;
		//spec.format = AUDIO_FORMAT;
		//spec.freq = SAMPLE_FREQUENCY;

		//if (!Mix_OpenAudio(0, &spec))
		//	THROW_CRITICAL(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_CreateMixer_Failed());

		//// Sau khi khởi tạo, đặt master volume
		//set_volume(master);

		Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
		Mix_AllocateChannels(max_channels);

		if (!Mix_OpenAudio(0, &spec))
			THROW_CRITICAL(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_CreateMixer_Failed());

		set_volume(master_volume);
	}
	Mixer::~Mixer() { quit(); }
}