#include "audio/memory.h" // Header
#include <SDL3_mixer/SDL_mixer.h>
#include <ranges>
#include "exceptions.h"
#include "logging.h"

//! AudioMemory<Music>
void AudioMemory<Music>::free(const std::string& name)
{
	if (auto it = this->find(name); it != this->end()) 
	{
		Mix_FreeMusic(it->second);
		this->erase(it);
	}
}
void AudioMemory<Music>::free_all()
{
	for (const auto& val : *this | std::views::values)
		Mix_FreeMusic(val);
	this->clear();
}
Music AudioMemory<Music>::load(const char* file_path, const std::string& name)
{
	Mix_Music* audio = Mix_LoadMUS(file_path);
	if (!audio)
		THROW_ERROR(SDLExceptions::Audio::SDL_Audio_LoadMusic_Failed(file_path));

	insert_or_assign(name, audio);
	return audio;
}

//! AudioMemory<Effects
void AudioMemory<Effect>::free(const std::string& name)
{
	if (auto it = this->find(name); it != this->end()) 
	{
		Mix_FreeChunk(it->second);
		this->erase(it);
	}
}
void AudioMemory<Effect>::free_all()
{
	for (const auto& val : *this | std::views::values)
		Mix_FreeChunk(val);
	this->clear();
}
Effect AudioMemory<Effect>::load(const char* file_path, const std::string& name)
{
	Mix_Chunk* audio = Mix_LoadWAV(file_path);
	if (!audio)
		THROW_ERROR(SDLExceptions::Audio::SDL_Audio_LoadEffect_Failed(file_path));

	insert_or_assign(name, audio);
	return audio;
}