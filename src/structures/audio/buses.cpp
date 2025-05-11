#include "structures/audio/buses.h" // Header
#include "structures/type.hpp"
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "utilities.hpp"
#include "structures/audio/memory.h"

namespace Structures::Audio
{
	//! Bus<Music>
	const float& Bus<Music>::get_volume() const { return *volume; }
	float Bus<Music>::set_volume(float percent) const
	{
		percent = std::clamp(percent, 0.0f, 1.0f);
		*volume = percent;

		// Hiện tại set volume music của SDL đang bị lỗi
		return Utilities::Math::to_percent(
			Mix_VolumeMusic(
				Utilities::Math::to_value(percent, 0, MIX_MAX_VOLUME)),
			0, MIX_MAX_VOLUME);
	}
	void Bus<Music>::play(const MusicMemory::Item& music, const bool pause)
	{
		if (!music.parent || music.item == music.parent->items.end()) return;

		if (has_song_playing()) stop();
		if (!Mix_PlayMusic(music.item->second, 0))
		{
			LOG_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_PlayMusic_Failed(music.item->first));
			return;
		}
		if (pause)
		{
			Bus::pause();
			seek(0);
		}
		current = music;
	}
	const MusicMemory::Item& Bus<Music>::get_current() const
	{
		return current;
	}
	int64_t Bus<Music>::get_position() const
	{
		return static_cast<int64_t>(std::round(Mix_GetMusicPosition(current.item->second) * 1000.0));
	}
	void Bus<Music>::seek(const int64_t& new_pos)
	{
		if (double new_pos_sec = static_cast<double>(new_pos) / 1000.0; 
			!Mix_SetMusicPosition(new_pos_sec))
			LOG_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_SetMusicPosition_Failed(new_pos_sec));
	}
	bool Bus<Music>::has_song_playing() { return (Mix_PlayingMusic() != 0); }
	bool Bus<Music>::is_playing() { return has_song_playing() && (Mix_PausedMusic() == 1); }
	void Bus<Music>::pause() { Mix_PauseMusic(); }
	void Bus<Music>::resume() { Mix_ResumeMusic(); }
	void Bus<Music>::stop()
	{
		current.parent = nullptr;
		return Mix_HaltMusic();
	}
	Bus<Music>::Bus(float* volume) : volume(volume) {}

	//! Bus<Effects>
	const float& Bus<Effect>::get_volume() const { return *volume; }
	float Bus<Effect>::get_volume(const EffectMemory::Item& sound) const
	{
		if (!sound.is_valid()) return -1;
		if (Utilities::Math::is_equal(*volume, 0.0f)) return 0;

		return static_cast<float>(Utilities::Math::to_percent(
			Mix_VolumeChunk(sound.item->second, -1),
			0, MIX_MAX_VOLUME)) / *volume;
	}
	float Bus<Effect>::get_volume(const int& channel) const
	{
		if (Utilities::Math::is_equal(*volume, 0.0f)) return 0;

		return Utilities::Math::to_percent(
			Mix_Volume(channel, -1),
			0, MIX_MAX_VOLUME) / *volume;
	}
	float Bus<Effect>::set_volume(const float& percent) const
	{
		const float previous_volume = get_volume();
		const int channel_num = Mix_AllocateChannels(-1);
		std::vector<float> volume_percent(channel_num);
		for (int channel = 0; channel < channel_num; ++channel)
			volume_percent[channel] = get_volume(channel);

		*volume = std::clamp(percent, 0.0f, 1.0f);
		for (int channel = 0; channel < channel_num; ++channel)
			set_volume(channel, volume_percent[channel]);
		return previous_volume;
	}
	float Bus<Effect>::set_volume(const EffectMemory::Item& sound, float percent) const
	{
		if (!sound.is_valid()) return -1;
		percent = std::clamp(percent, 0.0f, 1.0f);
		const auto old_volume = get_volume(sound);

		Mix_VolumeChunk(sound.item->second, Utilities::Math::to_value(percent * *volume, 0, MIX_MAX_VOLUME));
		return old_volume;
	}
	float Bus<Effect>::set_volume(const int& channel, float percent) const
	{
		percent = std::clamp(percent, 0.0f, 1.0f);
		const auto old_volume = get_volume(channel);

		Mix_Volume(channel, Utilities::Math::to_value(percent * *volume, 0, MIX_MAX_VOLUME));
		return old_volume;
	}
	int Bus<Effect>::play(const EffectMemory::Item& sound, const std::optional<float>& volume) const
	{
		if (!sound.parent || sound.item == sound.parent->data.end()) return -1;

		const int32_t channel = Mix_PlayChannel(-1, sound.item->second, 0);
		if (channel < 0) LOG_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_PlayEffect_Failed(sound.item->first));
		else if (volume.has_value()) set_volume(channel, volume.value());
		return channel;
	}
	void Bus<Mix_Chunk*>::stop(const int& channel)
	{
		Mix_HaltChannel(channel);
	}

	Bus<Effect>::Bus(float* volume) : volume(volume) {}
}