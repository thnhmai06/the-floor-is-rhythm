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
		float volume = 1.0f;
		MusicMemory::Item current;

	public:
		[[nodiscard]] float get_volume() const;
		float set_volume(float percent);
		void play(const MusicMemory::Item& music);
		const MusicMemory::Item& get_current() const;
		int64_t get_position() const;
		static void seek(const int64_t& new_pos);
		static bool has_song_playing();
		static bool is_playing();
		static void pause();
		static void resume();
		void stop();
	};

	template<>
	struct Bus<Effect>
	{
	protected:
		float volume = 1.0f;

	public:
		[[nodiscard]] float get_volume() const;
		[[nodiscard]] float get_volume(const EffectMemory::Item& sound) const;
		[[nodiscard]] float get_volume(const int& channel) const;
		float set_volume(const float& percent);
		float set_volume(const EffectMemory::Item& sound, float percent) const;
		float set_volume(const int& channel, float percent) const;
		int play(const EffectMemory::Item& sound, const std::optional<float>& volume = std::nullopt) const;
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