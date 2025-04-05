#include "structures/audio/buses.h" // Header
#include "config.h"
#include "template.h"
#include "logger/logging.h"
#include "logger/exceptions.h"
#include "utilities.h"

using namespace Utilities::Audio;
using UserConfig::Volume::music, UserConfig::Volume::hitsound;

//! Structures::Audio::Buses
using namespace Structures::Audio::Buses;
// ::AudioBus<Music>
int32_t AudioBus<Music>::set_volume(const int32_t value)
{
	if (value >= 0) volume = value;
	return get_real_volume(Mix_VolumeMusic(get_volume(value)));
}
void AudioBus<Music>::play(const std::string& name) const
{
	if (has_song_playing()) stop();

	const auto it = memory.find(name);
	if (it == memory.end()) 
		THROW_ERROR(SDLExceptions::Audio::SDL_Audio_PlayMusic_Failed(name));

	if (!Mix_PlayMusic(it->second, -1))
		THROW_ERROR(SDLExceptions::Audio::SDL_Audio_PlayMusic_Failed(name));
}
bool AudioBus<Music>::has_song_playing() { return (Mix_PlayingMusic() != 0); }
bool AudioBus<Music>::is_playing() { return has_song_playing() && (Mix_PausedMusic() == 1); }
void AudioBus<Music>::pause() { Mix_PauseMusic(); }
void AudioBus<Music>::resume() { Mix_ResumeMusic(); }
void AudioBus<Music>::stop() { return Mix_HaltMusic(); }

// ::AudioBus<Effects>
int32_t AudioBus<Effect>::set_volume(const int32_t value)
{
	if (value >= 0) volume = value;
	return get_real_volume(Mix_Volume(-1, get_volume(value)));
}
int32_t AudioBus<Effect>::set_effect_volume(const std::string& name, const int32_t value) const
{
	const auto it = memory.find(name);
	if (it == memory.end()) return -1;
	Effect audio = it->second;
	if (value < 0)
		return get_real_volume(Mix_VolumeChunk(audio, -1));
	return get_real_volume(Mix_VolumeChunk(audio, get_volume(value)));
}
int32_t AudioBus<Effect>::play(const std::string& name) const
{
	const auto it = memory.find(name);
	if (it == memory.end())
		THROW_ERROR(SDLExceptions::Audio::SDL_Audio_PlayEffect_Failed(name));

	const int32_t result = Mix_PlayChannel(-1, it->second, 0);
	if (result < 0)
		THROW_ERROR(SDLExceptions::Audio::SDL_Audio_PlayEffect_Failed(name));
	return result;
}