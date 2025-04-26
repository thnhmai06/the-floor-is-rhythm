#include "structures/audio/buses.h" // Header
#include "structures/type.hpp"
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "utilities.hpp"
#include "structures/audio/memory.h"

namespace Structures::Audio
{
	//! Bus<Music>
	float Bus<Music>::get_volume() const { return volume; }
	float Bus<Music>::set_volume(float percent)
	{
		if (percent < 0) percent = 0;
		else if (percent > 1) percent = 1;

		volume = percent;
		return Utilities::Math::to_percent(
			Mix_VolumeMusic(
				Utilities::Math::to_value(percent, 0, MIX_MAX_VOLUME)),
			0, MIX_MAX_VOLUME);
	}
	void Bus<Music>::play(const MusicMemory::Item& music)
	{
		if (!music.parent || music.item == music.parent->items.end()) return;

		if (has_song_playing()) stop();
		if (!Mix_PlayMusic(music.item->second, -1))
			LOG_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_PlayMusic_Failed(music.item->first));
	}
	bool Bus<Music>::has_song_playing() { return (Mix_PlayingMusic() != 0); }
	bool Bus<Music>::is_playing() { return has_song_playing() && (Mix_PausedMusic() == 1); }
	void Bus<Music>::pause() { Mix_PauseMusic(); }
	void Bus<Music>::resume() { Mix_ResumeMusic(); }
	void Bus<Music>::stop() { return Mix_HaltMusic(); }
	Bus<Music>::Bus(const float& volume) { set_volume(volume); }

	//! Bus<Effects>
	float Bus<Effect>::get_volume() const { return volume; }
	float Bus<Effect>::get_volume(const EffectMemory::Item& sound) const
	{
		if (!sound.is_valid()) return -1;
		if (Utilities::Math::is_equal(volume, 0.0f)) return 0;

		return Utilities::Math::to_percent(
			Mix_VolumeChunk(sound.item->second, -1),
			0, MIX_MAX_VOLUME) / volume;
	}
	float Bus<Effect>::get_volume(const int& channel) const
	{
		if (Utilities::Math::is_equal(volume, 0.0f)) return 0;

		return Utilities::Math::to_percent(
			Mix_Volume(channel, -1),
			0, MIX_MAX_VOLUME) / volume;
	}
	float Bus<Effect>::set_volume(const float& percent)
	{
		const float previous_volume = get_volume();
		const int channel_num = Mix_AllocateChannels(-1);
		std::vector<float> volume_percent(channel_num);
		for (int channel = 0; channel < channel_num; ++channel)
			volume_percent[channel] = get_volume(channel);

		volume = std::clamp(percent, 0.0f, 1.0f);
		for (int channel = 0; channel < channel_num; ++channel)
			set_volume(channel, volume_percent[channel]);
		return previous_volume;
	}
	float Bus<Effect>::set_volume(const EffectMemory::Item& sound, float percent) const
	{
		if (!sound.is_valid()) return -1;
		percent = std::clamp(percent, 0.0f, 1.0f);
		const auto old_volume = get_volume(sound);

		Mix_VolumeChunk(sound.item->second, Utilities::Math::to_value(percent * volume, 0, MIX_MAX_VOLUME));
		return old_volume;
	}
	float Bus<Effect>::set_volume(const int& channel, float percent) const
	{
		percent = std::clamp(percent, 0.0f, 1.0f);
		const auto old_volume = get_volume(channel);

		Mix_Volume(channel, Utilities::Math::to_value(percent * volume, 0, MIX_MAX_VOLUME));
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
	Bus<Mix_Chunk*>::Bus(const float& volume) { set_volume(volume); }

	//! EffectManager
	/*const float& EffectManager::get_volume() const { return volume; }
	float EffectManager::set_volume(const float& percent)
	{
		const auto previous_volume = get_volume();
		std::vector<float> bus_volume(buses.size());
		for (size_t bus = 0; bus < buses.size(); ++bus)
			bus_volume[bus] = buses[bus]->get_volume();

		volume = std::clamp(percent, 0.0f, 1.0f);
		for (size_t bus = 0; bus < buses.size(); ++bus)
			buses[bus]->set_volume(bus_volume[bus] * volume);
		return previous_volume;
	}
	EffectManager::EffectManager(const float& volume, CONTAINER buses) : buses(std::move(buses))
	{
		set_volume(volume);
	}*/
}