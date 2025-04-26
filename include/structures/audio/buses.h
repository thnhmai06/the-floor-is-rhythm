#pragma once
#include <SDL3_mixer/SDL_mixer.h>
#include "structures/audio/memory.h"
#include "structures/type.hpp"

namespace Structures::Audio
{
	template <AudioPtrType AudioPtr>
	struct Bus;

	//! CHỈ CÓ DUY NHẤT 1 BUS MUSIC TỒN TẠI!
	template<>
	struct Bus<Music>
	{
	protected:
		float volume;

	public:
		[[nodiscard]] float get_volume() const;
		float set_volume(float percent);
		static void play(const MusicMemory::Item& music);
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
	protected:
		float volume;

	public:
		[[nodiscard]] float get_volume() const;
		[[nodiscard]] float get_volume(const EffectMemory::Item& sound) const;
		[[nodiscard]] float get_volume(const int& channel) const;
		float set_volume(const float& percent);
		float set_volume(const EffectMemory::Item& sound, float percent) const;
		float set_volume(const int& channel, float percent) const;
		int play(const EffectMemory::Item& sound, const std::optional<float>& volume = std::nullopt) const;

		explicit Bus(const float& volume);
	};

	/*struct EffectManager
	{
	protected:
		float volume;

	public:
		using CONTAINER = std::vector<Bus<Effect>*>;
		CONTAINER buses;
		[[nodiscard]] const float& get_volume() const;
		float set_volume(const float& percent);

		explicit EffectManager(const float& volume, CONTAINER buses = {});
	};*/
}