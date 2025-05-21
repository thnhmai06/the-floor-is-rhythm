#include "engine/audio/mixer.h" // Header
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "utilities.h"

namespace Engine::Audio
{
	const double& Mixer::get_volume() const { return volume.percent; }
	double Mixer::set_volume(double percent)
	{
		percent = std::clamp(percent, 0.0, 1.0);

		const auto vl = static_cast<uint8_t>(Mix_MasterVolume(volume.get_value(true, percent)));
		volume.percent = percent;
		return volume.get_percent(true, vl);
	}
	void Mixer::stop_all()
	{
		music.stop();
		Bus::Bus<Effect>::stop();
	}
	Mixer::Mixer(const MIX_InitFlags& flags, const SDL_AudioSpec& spec, 
		const double& master_volume, const double& music_volume,
		const double& effect_volume, const int32_t& max_channels)
	: spec(spec)
	{
		Mix_Init(flags);
		if (!Mix_OpenAudio(0, &this->spec))
			THROW_CRITICAL(Logging::Exceptions::Engine::Audio::SDL_Audio_CreateMixer_Failed());

		Mix_AllocateChannels(max_channels);
		set_volume(master_volume);
		music.set_volume(music_volume);
		effect.set_volume(effect_volume);
	}
	Mixer::~Mixer() { Mix_Quit(); }
}