#pragma once
#include <SDL3_mixer/SDL_mixer.h>
#include "structures/audio/memory.h"
#include "template.h"

namespace Structures::Audio::Buses
{
	using namespace Structures::Audio::Memory;

	template <AudioPtrType AudioPtr>
	struct AudioBus
	{
		AudioMemory<AudioPtr> memory;
	};

	template<>
	struct AudioBus<Music>
	{
		AudioMemory<Music> memory;
		int32_t volume;

		int32_t set_volume(int32_t value = -1);
		void play(const std::string& name) const;
		static bool has_song_playing();
		static bool is_playing();
		static void pause();
		static void resume();
		static void stop();

		explicit AudioBus(const int32_t value = MIX_MAX_VOLUME) : volume(value) {}
	};

	template<>
	struct AudioBus<Effect>
	{
		AudioMemory<Effect> memory;
		int32_t volume;

		int32_t set_volume(int32_t value = -1);
		int32_t set_effect_volume(const std::string& name, int32_t value = -1) const;
		int32_t play(const std::string& name) const;

		explicit AudioBus(const int32_t value = MIX_MAX_VOLUME) : volume(value) {}
	};
}