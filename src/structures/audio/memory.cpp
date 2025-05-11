#include "structures/audio/memory.h" // Header
#include <SDL3_mixer/SDL_mixer.h>
#include <ranges>
#include "logging/exceptions.h"
#include "logging/logger.h"

namespace Structures::Audio
{
	//! MusicMemory
	// ::Item
	void MusicMemory::Item::free()
	{
		if (item != parent->items.end() && parent)
		{
			item = parent->items.end();
			parent->free(item->first);
		}
	}
	MusicMemory::Item::Item(MusicMemory* parent, CONTAINER::const_iterator item) : parent(parent), item(std::move(item))
	{
	}

	// ::
	MusicMemory::Item MusicMemory::get(const std::string& name)
	{
		if (const auto& it = items.find(name); it != items.end())
		{
			return Item{ this, it };
		}
		return Item{ nullptr, items.end() };
	}
	void MusicMemory::free(const std::string& name)
	{
		if (const auto it = items.find(name); it != items.end())
		{
			Mix_FreeMusic(it->second);
			items.erase(it);
		}
	}
	void MusicMemory::free_all()
	{
		for (const auto& val : items | std::views::values)
			Mix_FreeMusic(val);
		items.clear();
	}

	MusicMemory::Item MusicMemory::load(const std::filesystem::path& file_path, const std::string& name)
	{
		Mix_Music* audio = Mix_LoadMUS(file_path.string().c_str());
		if (!audio)
			THROW_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_LoadMusic_Failed(file_path));

		return { this, items.insert_or_assign(name, audio).first };
	}

	//! EffectMemory
	// ::Item
	EffectMemory::Item::Item(EffectMemory* parent, CONTAINER::const_iterator item) : parent(parent), item(std::move(item))
	{
	}
	void EffectMemory::Item::free()
	{
		if (item != parent->data.end() && parent)
		{
			item = parent->data.end();
			parent->free(item->first);
		}
	}
	bool EffectMemory::Item::is_valid() const
	{
		return (parent && item != parent->data.end() && item->second);
	}

	//::
	EffectMemory::Item EffectMemory::find(const std::string& name, const bool no_log_not_found)
	{
		if (const auto& it = data.find(name); it != data.end())
		{
			return Item{ this, it };
		}
		if (!no_log_not_found) LOG_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_NotFound(name));
		return Item{ nullptr, data.end() };
	}
	void EffectMemory::free(const std::string& name)
	{
		if (const auto it = data.find(name); it != data.end())
		{
			Mix_FreeChunk(it->second);
			data.erase(it);
		}
	}
	void EffectMemory::free_all()
	{
		for (const auto& val : data | std::views::values)
			Mix_FreeChunk(val);
		data.clear();
	}
	EffectMemory::Item EffectMemory::load(const std::filesystem::path& file_path, const std::string& name, const bool override)
	{
		if (!override && data.contains(name))
			return {};

		Mix_Chunk* audio = Mix_LoadWAV(file_path.string().c_str());
		if (!audio)
		{
			LOG_ERROR(Logging::Exceptions::SDLExceptions::Audio::SDL_Audio_LoadEffect_Failed(file_path));
			return {};
		}

		return { this, data.insert_or_assign(name, audio).first };
	}
	void EffectMemory::load(
		const std::filesystem::path& folder_path,
		const std::filesystem::path& root_folder,
		const bool recursive, const bool name_include_extension, const bool relative_path_name, const bool override,
		const std::unordered_set<std::filesystem::path>& blacklist)
	{
		if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path))
			for (const auto& entry : std::filesystem::recursive_directory_iterator(folder_path))
			{
				if (recursive && std::filesystem::is_directory(entry))
					load(entry, root_folder, recursive, name_include_extension, relative_path_name, override, blacklist);
				else if (std::filesystem::is_regular_file(entry))
				{
					const auto extension = entry.path().extension().string();
					if (!Format::Skin::Sound::SUPPORT_EXTENSIONS.contains(extension)) continue;

					std::filesystem::path path_name = entry.path().lexically_relative(root_folder);
					if (!relative_path_name)
						path_name = path_name.filename();
					if (!name_include_extension)
						path_name.replace_extension();
					const auto name = path_name.generic_string();

					if (blacklist.contains(name)) 
						continue;
					if (!override && data.contains(name)) continue;
					load(entry, name);
				}
			}
	}
}
