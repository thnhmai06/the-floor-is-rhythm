#pragma once
#include <SDL3/SDL_audio.h>

#include "structures/config.h"
#include "structures/audio/buses.h"
#include "structures/type.hpp"

namespace Structures::Audio
{
	struct Mixer
	{
	private:
		float* volume;

	public:
		SDL_AudioSpec spec;
		Bus<Music> music;
		Bus<Effect> effect;

		[[nodiscard]] const float& get_volume() const;
		float set_volume(const float& percent) const;
		static void quit();

		Mixer(const SDL_AudioSpec& spec, Config::UserConfig::Audio::Volume& volume, const int32_t& max_channels = 8);
		~Mixer();
	};
}