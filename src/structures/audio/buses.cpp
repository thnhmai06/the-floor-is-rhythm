#include "structures/audio/buses.h" // Header
#include "config.h"
#include "structures/types.h"
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "utilities.hpp"

namespace Structures::Audio
{
	// Bus<Music>
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
	void Bus<Music>::play(const Memory<Music>::Item& music)
	{
		if (!music.parent || music.item != music.parent->items.end()) return;

		if (has_song_playing()) stop();
		if (!Mix_PlayMusic(music.item->second, -1))
			THROW_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_PlayMusic_Failed(music.item->first));
	}
	bool Bus<Music>::has_song_playing() { return (Mix_PlayingMusic() != 0); }
	bool Bus<Music>::is_playing() { return has_song_playing() && (Mix_PausedMusic() == 1); }
	void Bus<Music>::pause() { Mix_PauseMusic(); }
	void Bus<Music>::resume() { Mix_ResumeMusic(); }
	void Bus<Music>::stop() { return Mix_HaltMusic(); }
	Bus<Music>::Bus(const float& volume) { set_volume(volume); }

	// Bus<Effects>
	float Bus<Effect>::get_volume() const { return volume; }
	float Bus<Effect>::get_volume(const Memory<Effect>::Item& sound)
	{
		if (!sound.parent || sound.item != sound.parent->items.end())
			return -1;

		return Utilities::Math::to_percent(
			Mix_VolumeChunk(sound.item->second, -1),
			0, MIX_MAX_VOLUME);
	}
	float Bus<Effect>::set_volume(float percent)
	{
		if (percent < 0) percent = 0;
		else if (percent > 1) percent = 1;

		volume = percent;
		return Utilities::Math::to_percent(
			Mix_Volume(-1, Utilities::Math::to_value(percent, 0, MIX_MAX_VOLUME)),
			0, MIX_MAX_VOLUME);
	}
	float Bus<Effect>::set_volume(const Memory<Effect>::Item& sound, float percent)
	{
		if (!sound.parent || sound.item != sound.parent->items.end())
			return -1;

		if (percent < 0) percent = 0;
		else if (percent > 1) percent = 1;

		return Utilities::Math::to_percent(
			Mix_VolumeChunk(sound.item->second, 
				Utilities::Math::to_value(percent, 0, MIX_MAX_VOLUME)),
			0, MIX_MAX_VOLUME);
	}
	int32_t Bus<Effect>::play(const Memory<Effect>::Item& sound)
	{
		if (!sound.parent || sound.item != sound.parent->items.end()) 
			return -1;

		const int32_t result = Mix_PlayChannel(-1, sound.item->second, 0);
		if (result < 0)
			THROW_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_PlayEffect_Failed(sound.item->first));
		return result;
	}
	Bus<Mix_Chunk*>::Bus(const float& volume) { set_volume(volume); }
}
