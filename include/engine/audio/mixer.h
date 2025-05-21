#pragma once
#include <SDL3/SDL_audio.h>
#include "engine/audio/buses.h"

namespace Engine::Audio
{
	struct Mixer
	{
	private:
		PercentValue<uint8_t> volume{ 0, MIX_MAX_VOLUME };

	public:
		SDL_AudioSpec spec;
		Bus::Bus<Music> music;
		Bus::Bus<Effect> effect;

		[[nodiscard]] const double& get_volume() const;
		double set_volume(double percent);
		void stop_all();

		Mixer(const MIX_InitFlags& flags, const SDL_AudioSpec& spec, 
			const double& master_volume, const double& music_volume,
			const double& effect_volume, const int32_t& max_channels = 8);
		~Mixer();
	};
}