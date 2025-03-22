#include "audio/memory.h" // Header

#include <ranges>

//! Music
void AudioMemory::Music::free(const std::string& name)
{
	auto it = this->find(name);
	if (it != this->end()) {
		Mix_FreeMusic(it->second);
		this->erase(it);
	}
}
void AudioMemory::Music::free_all()
{
	for (const auto& val : *this | std::views::values)
		Mix_FreeMusic(val);
	this->clear();
}
Mix_Music* AudioMemory::Music::load(const char* file_path, const std::string& name)
{
	Mix_Music* audio = Mix_LoadMUS(file_path);
	if (!audio) {
		// TODO: Xử lý lỗi không tải được audio
		return nullptr;
	}
	insert_or_assign(name, audio);
	return audio;
}

//! Effects
void AudioMemory::Effects::free(const std::string& name)
{
	auto it = this->find(name);
	if (it != this->end()) {
		Mix_FreeChunk(it->second);
		this->erase(it);
	}
}
void AudioMemory::Effects::free_all()
{
	for (const auto& val : *this | std::views::values)
		Mix_FreeChunk(val);
	this->clear();
}
Mix_Chunk* AudioMemory::Effects::load(const char* file_path, const std::string& name)
{
	Mix_Chunk* audio = Mix_LoadWAV(file_path);
	if (!audio) {
		// TODO: Xử lý lỗi không tải được audio
		return nullptr;
	}
	insert_or_assign(name, audio);
	return audio;
}