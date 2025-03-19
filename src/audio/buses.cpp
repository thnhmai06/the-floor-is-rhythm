#include "audio/buses.h" // Header
#include "audio/memory.h"
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
bool AudioBuses::Music::load(const char* file_path)
{
	return (memory.load(file_path) != nullptr);
}
int32_t AudioBuses::Music::play(const char* file_path, const bool while_playing_another) const
{
	if (!while_playing_another && Mix_PlayingMusic() == 1)
		return -1;

	auto it = memory.find(file_path);
	if (it == memory.end())
		return -1;

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
int32_t AudioBuses::Effects::set_effect_volume(const char* file_name, const int32_t value) const
{
	auto it = memory.find(file_name);
	if (it == memory.end()) return -1;
	Mix_Chunk* audio = it->second;
	if (value < 0)
		return get_real_volume(Mix_VolumeChunk(audio, -1));
	return get_real_volume(Mix_VolumeChunk(audio, get_volume(value)));
}
bool AudioBuses::Effects::load(const char* file_path)
{
	return (memory.load(file_path) != nullptr);
}
int32_t AudioBuses::Effects::play(const char* file_path) const
{
	auto it = memory.find(file_path);
	if (it == memory.end())
		return -1;
	Mix_Chunk* audio = it->second;
	return Mix_PlayChannel(-1, audio, 0);
}
AudioBuses::Effects::Effects() { set_volume(HITSOUND_VOLUME); }
