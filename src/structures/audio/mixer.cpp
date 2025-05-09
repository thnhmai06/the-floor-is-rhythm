#include "structures/audio/mixer.h" // Header
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "utilities.hpp"

namespace Structures::Audio
{
	float Mixer::get_volume() const { return volume; }
	float Mixer::set_volume(const float& percent)
	{
		volume = std::clamp(percent, 0.0f, 1.0f);

		return Utilities::Math::to_percent(
			Mix_MasterVolume(
				Utilities::Math::to_value(volume, 0, MIX_MAX_VOLUME)),
			0, MIX_MAX_VOLUME);
	}
	// ::Mixer
	void Mixer::quit() { Mix_Quit(); }
	Mixer::Mixer(const SDL_AudioSpec& spec, const float& master_volume, const float& music_volume,
		const float& effects_volume, const int32_t& max_channels) : spec(spec)
	{
		Mix_Init(0xff);
		if (!Mix_OpenAudio(0, &spec))
			THROW_CRITICAL(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_CreateMixer_Failed());

		Mix_AllocateChannels(max_channels);
		set_volume(master_volume);
		music.set_volume(music_volume);
		effect.set_volume(effects_volume);
	}
	Mixer::~Mixer() { quit(); }
}