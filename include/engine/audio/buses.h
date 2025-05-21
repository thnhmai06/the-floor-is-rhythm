#pragma once
#include "engine/audio/type.h"
#include "engine/audio/memory.h"
#include "engine/structures.h"

namespace Engine::Audio::Bus
{
	using namespace Engine::Audio::Memory;

	template <AudioTypename AudioType>
	struct Bus;

	//! CHỈ CÓ DUY NHẤT 1 BUS MUSIC TỒN TẠI!
	template<>
	struct Bus<Music>
	{
	protected:
		PercentValue<uint8_t> volume{ 0, MIX_MAX_VOLUME };
		MusicMemory::Item current;

	public:
		[[nodiscard]] const double& get_volume() const;
		double set_volume(double percent);
		void play(const MusicMemory::Item& music, bool pause = false);
		const MusicMemory::Item& get_current();
		int64_t get_position();
		static void seek(const int64_t& new_pos);
		static bool has_song_playing();
		static bool is_playing();
		static void pause();
		static void resume();
		void stop();

		explicit Bus(const double& volume = 1.0);
	};

	template<>
	struct Bus<Effect>
	{
	protected:
		PercentValue<uint8_t> volume{ 0, MIX_MAX_VOLUME };

	private:
		PercentValue<uint8_t> sound_volume{ 0, MIX_MAX_VOLUME }; // virtual

	public:
		[[nodiscard]] const double& get_volume() const;
		[[nodiscard]] double get_volume(const EffectMemory::Item& sound) const;
		[[nodiscard]] double get_volume(const int& channel) const;
		double set_volume(const double& percent);
		double set_volume(const EffectMemory::Item& sound, double percent) const;
		double set_volume(const int& channel, double percent) const;
		int play(const EffectMemory::Item& sound, const std::optional<double>& volume = std::nullopt) const;
		static void stop(const int& channel = -1);

		explicit Bus(const double& volume = 1.0);
	};
}