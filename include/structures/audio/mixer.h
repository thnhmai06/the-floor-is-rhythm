#pragma once
#include <SDL3/SDL_audio.h>
#include "structures/audio/buses.h"
#include "structures/type.hpp"

namespace Structures::Audio
{
	struct Mixer
	{
	private:
		float volume;

	public:
		SDL_AudioSpec spec;
		Bus<Music> music;
		Bus<Effect> effect;

		[[nodiscard]] float get_volume() const;
		float set_volume(const float& percent);
		static void quit();

		Mixer(const SDL_AudioSpec& spec, const float& master_volume, const float& music_volume,
		      const float& effects_volume, const int32_t& max_channels = 8);
		~Mixer();
	};
}