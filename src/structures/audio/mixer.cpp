#include "structures/audio/mixer.h" // Header
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "utilities.hpp"

namespace Structures::Audio
{
	const float& Mixer::get_volume() const { return *volume; }
	float Mixer::set_volume(const float& percent) const
	{
		*volume = std::clamp(percent, 0.0f, 1.0f);

		return Utilities::Math::to_percent(
			Mix_MasterVolume(
				Utilities::Math::to_value(*volume, 0, MIX_MAX_VOLUME)),
			0, MIX_MAX_VOLUME);
	}
	// ::Mixer
	void Mixer::quit() { Mix_Quit(); }
	Mixer::Mixer(const SDL_AudioSpec& spec, Config::UserConfig::Audio::Volume& volume, const int32_t& max_channels)
	: volume(&volume.master), spec(spec), music(&volume.music), effect(&volume.effect)
	{
		Mix_Init(0xff);
		if (!Mix_OpenAudio(0, &spec))
			THROW_CRITICAL(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_CreateMixer_Failed());

		Mix_AllocateChannels(max_channels);
		set_volume(*this->volume);
	}
	Mixer::~Mixer() { quit(); }
}