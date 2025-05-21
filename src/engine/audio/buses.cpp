#include "engine/audio/buses.h" // Header
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "utilities.h"

namespace Engine::Audio::Bus
{
	//! Bus<Music>
	const double& Bus<Music>::get_volume() const { return volume.percent; }
	double Bus<Music>::set_volume(double percent)
	{
		percent = std::clamp(percent, 0.0, 1.0);
		const auto vl = Mix_VolumeMusic(volume.get_value(true, percent));
		if (vl < 0)
		{
			LOG_ERROR(Logging::Exceptions::Engine::Audio::SDL_Audio_SetMusicVolume_Failed(current.item->first, volume.percent));
			return -1;
		}
		volume.percent = std::clamp(percent, 0.0, 1.0);
		return volume.get_percent(true, static_cast<uint8_t>(vl));
	}
	void Bus<Music>::play(const MusicMemory::Item& music, const bool pause)
	{
		if (!music.parent || music.item == music.parent->items.end()) return;

		if (has_song_playing()) stop();
		set_volume(volume.percent);
		if (!Mix_PlayMusic(music.item->second, 0))
		{
			LOG_ERROR(Logging::Exceptions::Engine::Audio::SDL_Audio_PlayMusic_Failed(music.item->first));
			return;
		}
		if (pause)
		{
			Bus::pause();
			seek(0);
		}
		current = music;
	}
	const MusicMemory::Item& Bus<Music>::get_current()
	{
		if (!has_song_playing())
			current.reset();
		return current;
	}
	int64_t Bus<Music>::get_position()
	{
		if (const auto current = get_current();
			current.is_valid())
			return static_cast<int64_t>(std::round(Mix_GetMusicPosition(current.item->second) * 1000.0));
		return -1;
	}
	void Bus<Music>::seek(const int64_t& new_pos)
	{
		if (const double new_pos_sec = static_cast<double>(new_pos) / 1000.0;
			!Mix_SetMusicPosition(new_pos_sec))
			LOG_ERROR(Logging::Exceptions::Engine::Audio::SDL_Audio_SetMusicPosition_Failed(new_pos_sec));
	}
	bool Bus<Music>::has_song_playing() { return Mix_PlayingMusic() != 0; }
	bool Bus<Music>::is_playing() { return has_song_playing() && (Mix_PausedMusic() == 1); }
	void Bus<Music>::pause() { Mix_PauseMusic(); }
	void Bus<Music>::resume() { Mix_ResumeMusic(); }
	void Bus<Music>::stop()
	{
		current.reset();
		return Mix_HaltMusic();
	}
	Bus<Music>::Bus(const double& volume)
	{
		set_volume(volume);
	}

	//! Bus<Effects>
	const double& Bus<Effect>::get_volume() const { return volume.percent; }
	double Bus<Effect>::get_volume(const EffectMemory::Item& sound) const
	{
		if (!sound.is_valid()) return -1;
		const auto vl = Mix_VolumeChunk(sound.item->second, -1);
		if (vl < 0)
			return -1;

		return sound_volume.get_percent(true, static_cast<uint8_t>(vl));
	}
	double Bus<Effect>::get_volume(const int& channel) const
	{
		const auto vl = Mix_Volume(channel, -1);
		if (vl < 0)
			return -1;

		return sound_volume.get_percent(true, static_cast<uint8_t>(vl));
	}
	double Bus<Effect>::set_volume(const double& percent)
	{
		const double previous_volume = get_volume();
		const auto channel_num = Mix_AllocateChannels(-1);
		std::vector<double> volume_percent(channel_num);
		for (int channel = 0; channel < channel_num; ++channel)
			volume_percent[channel] = get_volume(channel);

		volume.percent = std::clamp(percent, 0.0, 1.0);
		for (int channel = 0; channel < channel_num; ++channel)
			set_volume(channel, volume_percent[channel]);
		return previous_volume;
	}
	double Bus<Effect>::set_volume(const EffectMemory::Item& sound, double percent) const
	{
		if (!sound.is_valid()) return -1;
		percent = std::clamp(percent, 0.0, 1.0);

		const auto vl = Mix_VolumeChunk(sound.item->second, sound_volume.get_value(true, percent));
		if (vl < 0)
		{
			LOG_ERROR(Logging::Exceptions::Engine::Audio::SDL_Audio_SetEffectVolume_Failed(sound.item->first, percent));
			return -1;
		}

		return sound_volume.get_percent(true, static_cast<uint8_t>(vl));
	}
	double Bus<Effect>::set_volume(const int& channel, double percent) const
	{
		percent = std::clamp(percent, 0.0, 1.0);

		const auto vl = Mix_Volume(channel, sound_volume.get_value(true, percent));
		if (vl < 0)
		{
			LOG_ERROR(Logging::Exceptions::Engine::Audio::SDL_Audio_SetEffectVolume_Failed(channel, percent));
			return -1;
		}

		return sound_volume.get_percent(true, static_cast<uint8_t>(vl));
	}
	int Bus<Effect>::play(const EffectMemory::Item& sound, const std::optional<double>& volume) const
	{
		if (!sound.is_valid()) return -1;

		const int32_t channel = Mix_PlayChannel(-1, sound.item->second, 0);
		if (channel < 0)
		{
			LOG_ERROR(Logging::Exceptions::Engine::Audio::SDL_Audio_PlayEffect_Failed(sound.item->first));
		}
		else
		{
			set_volume(channel, volume.value_or(1.0));
		}
		return channel;
	}
	void Bus<Mix_Chunk*>::stop(const int& channel)
	{
		Mix_HaltChannel(channel);
	}
	Bus<Effect>::Bus(const double& volume)
	{
		sound_volume.parent = &this->volume;
		set_volume(volume);
	}
}