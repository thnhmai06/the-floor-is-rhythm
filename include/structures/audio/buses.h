#pragma once
#include <SDL3_mixer/SDL_mixer.h>
#include "structures/audio/memory.h"
#include "structures/types.h"

namespace Structures::Audio
{
	template <AudioPtrType AudioPtr>
	struct Bus;

	template<>
	struct Bus<Music>
	{
	private:
		float volume;

	public:
		[[nodiscard]] float get_volume() const;
		float set_volume(float percent);
		static void play(const Memory<Music>::Item& music);
		static bool has_song_playing();
		static bool is_playing();
		static void pause();
		static void resume();
		static void stop();

		explicit Bus(const float& volume);
	};

	template<>
	struct Bus<Effect>
	{
	private:
		float volume;

	public:
		[[nodiscard]] float get_volume() const;
		[[nodiscard]] static float get_volume(const Memory<Effect>::Item& sound);
		float set_volume(float percent);
		static float set_volume(const Memory<Effect>::Item& sound, float percent);
		static int32_t play(const Memory<Effect>::Item& sound);

		explicit Bus(const float& volume);
	};
}