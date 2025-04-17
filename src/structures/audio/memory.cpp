#include "structures/audio/memory.h" // Header
#include <SDL3_mixer/SDL_mixer.h>
#include <ranges>
#include "logging/exceptions.h"
#include "logging/logger.h"

namespace Structures::Audio
{
	// Memory::Item
	template <AudioPtrType AudioPtr>
	void Memory<AudioPtr>::Item::free()
	{
		if (item && parent)
		{
			parent->free(*item->first);
			item = nullptr;
		}
	}

	// Memory
	template <AudioPtrType AudioPtr>
	typename Memory<AudioPtr>::Item Memory<AudioPtr>::get(const std::string& name) const
	{
		if (const auto it = items.find(name); it != items.end())
			return { this, &it->first, &it->second };
		return { nullptr, nullptr, nullptr };
	}

	// Memory<Music>
	void Memory<Music>::free(const std::string& name)
	{
		if (const auto it = items.find(name); it != items.end())
		{
			Mix_FreeMusic(it->second);
			items.erase(it);
		}
	}
	void Memory<Music>::free_all()
	{
		for (const auto& val : items | std::views::values)
			Mix_FreeMusic(val);
		items.clear();
	}
	Memory<Music>::Item Memory<Music>::load(const char* file_path, const std::string& name)
	{
		Mix_Music* audio = Mix_LoadMUS(file_path);
		if (!audio)
			THROW_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_LoadMusic_Failed(file_path));

		return { .parent = this, .item = items.insert_or_assign(name, audio).first };
	}

	// Memory<Effects>
	void Memory<Effect>::free(const std::string& name)
	{
		if (const auto it = items.find(name); it != items.end())
		{
			Mix_FreeChunk(it->second);
			items.erase(it);
		}
	}
	void Memory<Effect>::free_all()
	{
		for (const auto& val : items | std::views::values)
			Mix_FreeChunk(val);
		items.clear();
	}
	Memory<Effect>::Item Memory<Effect>::load(const char* file_path, const std::string& name)
	{
		Mix_Chunk* audio = Mix_LoadWAV(file_path);
		if (!audio)
			THROW_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_LoadEffect_Failed(file_path));

		return { .parent = this, .item = items.insert_or_assign(name, audio).first };
	}
}