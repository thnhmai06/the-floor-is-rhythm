#include "audio/memory.h" // Header

//! Music
void AudioMemory::Music::free(const char* file_path)
{
	auto it = this->find(file_path);
	if (it != this->end()) {
		Mix_FreeMusic(it->second);
		this->erase(it);
	}
}
void AudioMemory::Music::free_all()
{
	for (const auto& item : *this)
		Mix_FreeMusic(item.second);
	this->clear();
}
Mix_Music* AudioMemory::Music::load(const char* file_path)
{
	auto it = this->find(file_path);
	if (it != this->end())
		return it->second;

	Mix_Music* audio = Mix_LoadMUS(file_path);
	if (!audio) {
		// TODO: Xử lý lỗi không tải được audio
		return nullptr;
	}
	this->insert({ file_path, audio });
	return audio;
}

//! Effects
void AudioMemory::Effects::free(const char* file_path)
{
	auto it = this->find(file_path);
	if (it != this->end()) {
		Mix_FreeChunk(it->second);
		this->erase(it);
	}
}
void AudioMemory::Effects::free_all()
{
	for (const auto& [key, value] : *this)
		Mix_FreeChunk(value);
	this->clear();
}
Mix_Chunk* AudioMemory::Effects::load(const char* file_path)
{
	auto it = this->find(file_path);
	if (it != this->end())
		return it->second;

	Mix_Chunk* audio = Mix_LoadWAV(file_path);
	if (!audio) {
		// TODO: Xử lý lỗi không tải được audio
		return nullptr;
	}
	this->insert({ file_path, audio });
	return audio;
}