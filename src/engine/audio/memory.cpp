#include "engine/audio/memory.h" // Header
#include <SDL3_mixer/SDL_mixer.h>
#include <ranges>
#include "logging/exceptions.h"
#include "logging/logger.h"

namespace Engine::Audio::Memory
{
	//! MusicMemory
	// ::Item
	void MusicMemory::Item::reset()
	{
		if (parent) item = parent->items.end();
		parent = nullptr;
	}
	bool MusicMemory::Item::is_valid() const
	{
		return (parent && item != parent->items.end() && item->second);
	}
	MusicMemory::Item::Item(
		const MusicMemory* parent,
		Container::const_iterator item)
		: parent(parent), item(std::move(item))
	{
	}

	// ::
	MusicMemory::Item MusicMemory::find(const std::string& name) const
	{
		if (const auto& it = items.find(name); it != items.end())
			return { this, it };
		return { nullptr, items.end() };
	}
	void MusicMemory::free(const std::string& name)
	{
		if (const auto it = items.find(name); it != items.end())
		{
			Mix_FreeMusic(it->second);
			items.erase(it);
		}
	}
	void MusicMemory::free(const Item& item)
	{
		if (item.is_valid() && item.parent == this)
			free(item.item->first);
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
		{
			LOG_ERROR(Logging::Exceptions::Engine::Audio::SDL_Audio_LoadMusic_Failed(file_path));
			return {};
		}

		return { this, items.insert_or_assign(name, audio).first };
	}
	MusicMemory::~MusicMemory() { free_all(); }

	//! EffectMemory
	// ::Item
	EffectMemory::Item::Item(
		const EffectMemory* parent,
		Container::const_iterator item)
		: parent(parent), item(std::move(item))
	{
	}
	bool EffectMemory::Item::is_valid() const
	{
		return (parent && item != parent->data.end() && item->second);
	}

	//::
	std::unordered_set<std::string_view> EffectMemory::SUPPORT_EXTENSIONS = { ".wav", ".mp3", ".ogg" };
	EffectMemory::Item EffectMemory::find(const std::string& name, const bool no_log_not_found) const
	{
		if (const auto& it = data.find(name); it != data.end())
		{
			return { this, it };
		}
		if (!no_log_not_found) LOG_ERROR(Logging::Exceptions::Engine::Audio::Engine_Audio_NotFound(name));
		return { nullptr, data.end() };
	}
	void EffectMemory::free(const std::string& name)
	{
		if (const auto it = data.find(name); it != data.end())
		{
			Mix_FreeChunk(it->second);
			data.erase(it);
		}
	}
	void EffectMemory::free(const Item& item)
	{
		if (item.is_valid() && item.parent == this)
			free(item.item->first);
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
			LOG_ERROR(Logging::Exceptions::Engine::Audio::SDL_Audio_LoadEffect_Failed(file_path));
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
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(folder_path))
			{
				if (recursive && std::filesystem::is_directory(entry))
					load(entry, root_folder, recursive, name_include_extension, relative_path_name, override, blacklist);
				else if (std::filesystem::is_regular_file(entry))
				{
					const auto extension = entry.path().extension().string();
					if (!SUPPORT_EXTENSIONS.contains(extension)) continue;

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
	EffectMemory::~EffectMemory() { free_all(); }
}
