#include "audio/buses.h" // Header
#include "config.h"
#include "utilities.h"

using namespace Utilities::Audio;
using namespace Config::Default::Volume;

//! Music
int32_t AudioBuses::Music::set_volume(const int32_t value)
{
	if (value >= 0) volume = value;
	return get_real_volume(Mix_VolumeMusic(get_volume(value)));
}
bool AudioBuses::Music::play(const std::string& name) const
{
	if (has_song_playing()) stop();

	const auto it = memory.find(name);
	if (it == memory.end()) return false;

	Mix_Music* audio = it->second;
	return Mix_PlayMusic(audio, -1);
}
bool AudioBuses::Music::has_song_playing() { return (Mix_PlayingMusic() != 0); }
bool AudioBuses::Music::is_playing() { return has_song_playing() && (Mix_PausedMusic() == 1); }
void AudioBuses::Music::pause() { Mix_PauseMusic(); }
void AudioBuses::Music::resume() { Mix_ResumeMusic(); }
void AudioBuses::Music::stop() { return Mix_HaltMusic(); }

AudioBuses::Music::Music() { set_volume(MUSIC_VOLUME); }

//! Effects
int32_t AudioBuses::Effects::set_volume(const int32_t value)
{
	if (value >= 0) volume = value;
	return get_real_volume(Mix_Volume(-1, get_volume(value)));
}
int32_t AudioBuses::Effects::set_effect_volume(const std::string& name, const int32_t value) const
{
	const auto it = memory.find(name);
	if (it == memory.end()) return -1;
	Mix_Chunk* audio = it->second;
	if (value < 0)
		return get_real_volume(Mix_VolumeChunk(audio, -1));
	return get_real_volume(Mix_VolumeChunk(audio, get_volume(value)));
}
int32_t AudioBuses::Effects::play(const std::string& name) const
{
	const auto it = memory.find(name);
	if (it == memory.end()) return -1;

	Mix_Chunk* audio = it->second;
	return Mix_PlayChannel(-1, audio, 0);
}

AudioBuses::Effects::Effects() { set_volume(HITSOUND_VOLUME); }