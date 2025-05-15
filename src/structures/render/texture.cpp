#include "structures/render/texture.h" // Header
#include <ranges>
#include <SDL3_image/SDL_image.h>
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "utilities.hpp"
#include "format/skin.h"

namespace Structures::Render::Texture
{
	//! Memory
	// ::
	const Memory::CONTAINER* Memory::get_data() const { return &data; }
	Memory::Item Memory::load(std::filesystem::path file_path, const std::string& name, const bool override)
	{
		if (!override && data.contains(name)) return {};
		file_path = Utilities::Path::normalize_path(file_path);

		SDL_Texture* texture = IMG_LoadTexture(this->renderer, file_path.string().c_str());
		if (!texture)
		{
			LOG_ERROR(Logging::Exceptions::SDLExceptions::Render::SDL_Render_LoadTexture_Failed(file_path));
			return {};
		}

		return load(texture, name, override);
	}
	Memory::Item Memory::load(SDL_Texture* texture, const std::string& name, const bool override)
	{
		if (!override && data.contains(name)) return {};
		if (data.contains(name))
			SDL_DestroyTexture(data[name]);

		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
		const auto& item = data.insert_or_assign(name, texture).first;
		return { item, this };
	}
	std::unordered_set<std::string> Memory::load(
		const std::filesystem::path& folder_path,
		const std::filesystem::path& root_folder,
		const bool recursive, const bool name_include_extension, const bool relative_path_name, const bool override,
		const std::unordered_set<std::string>& only_in)
	{
		std::unordered_set<std::string> changed_skin;
		if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path))
			for (const auto& entry : std::filesystem::recursive_directory_iterator(folder_path))
			{
				if (recursive && std::filesystem::is_directory(entry))
				{
					const auto result = load(entry, root_folder, recursive, name_include_extension, relative_path_name, override, only_in);
					changed_skin.insert(result.begin(), result.end());
				}
				else if (std::filesystem::is_regular_file(entry))
				{
					const auto extension = entry.path().extension().string();
					if (!Format::Skin::Image::SUPPORT_EXTENSIONS.contains(extension)) continue;

					std::filesystem::path path_name = entry.path().lexically_relative(root_folder);
					if (!relative_path_name)
						path_name = path_name.filename();
					if (!name_include_extension)
						path_name.replace_extension();
					const auto name = path_name.generic_string();

					if (!only_in.empty() && !only_in.contains(name)) continue;
					if (!override && data.contains(name)) continue;
					if (load(entry, name, override).is_valid())
						changed_skin.insert(name);
				}
			}
		return changed_skin;
	}
	Memory::Item Memory::find(const std::string& name, const bool no_log_not_found) const
	{
		if (const auto it = data.find(name); it != data.end())
			return { it, this };
		if (!no_log_not_found) LOG_ERROR(Logging::Exceptions::SDLExceptions::Render::SDL_Render_Texture_NotFound(name));
		return {};
	}
	Memory::Item Memory::create_new(const std::string& name, const SDL_Point& size, const bool override, 
		const SDL_PixelFormat& format, const SDL_TextureAccess& access)
	{
		const auto texture = SDL_CreateTexture(renderer, format, access, size.x, size.y);
		if (!texture)
		{
			LOG_ERROR(Logging::Exceptions::SDLExceptions::Render::SDL_Render_LoadTexture_Failed(name));
			return {};
		}

		return load(texture, name, override);
	}
	SDL_FPoint Memory::get_texture_size(const CONTAINER::const_iterator& texture)
	{
		float width, height;
		SDL_GetTextureSize(texture->second, &width, &height);
		return { width, height };
	}
	SDL_FPoint Memory::get_texture_size(const std::string& name) const
	{
		if (const auto it = data.find(name); it != data.end())
			return get_texture_size(it);
		return { 0, 0 };
	}
	Memory::CONTAINER::iterator Memory::rename_texture(const std::string& old_name, const std::string& new_name)
	{
		if (const auto item = data.find(old_name); item != data.end())
		{
			auto new_location = data.insert_or_assign(new_name, item->second).first;
			data.erase(item);
			return new_location;
		}
		return data.end();
	}
	Memory::CONTAINER::iterator Memory::move_texture(const std::string& name, Memory& to_memory)
	{
		if (const auto current = data.find(name); current != data.end())
		{
			auto new_location = to_memory.data.insert_or_assign(name, current->second).first;
			data.erase(current);
			return new_location;
		}
		return data.end();
	}
	void Memory::free_texture(const std::string& name)
	{
		if (const auto it = data.find(name); it != data.end())
		{
			if (it->second) SDL_DestroyTexture(it->second);
			data.erase(it);
		}
	}
	void Memory::free_all()
	{
		for (const auto& texture : data | std::views::values)
			SDL_DestroyTexture(texture);
		data.clear();
	}
	void Memory::clear() { free_all(); }
	Memory::Memory(SDL_Renderer* renderer) : renderer(renderer)
	{
	}
	Memory::~Memory() { free_all(); }

	// ::Item
	const std::string& Memory::Item::get_name() const { return item->first; }
	SDL_FPoint Memory::Item::get_size() const { return memory->get_texture_size(get_name()); }
	void Memory::Item::change_target(const std::string& new_target, const Memory* new_memory)
	{
		if (new_memory) memory = new_memory;
		if (!new_target.empty())
			item = memory->find(new_target).item;
	}
	bool Memory::Item::is_valid() const
	{
		return memory && item != memory->data.end() && item->second;
	}
	void Memory::Item::render(const SDL_FRect* src_rect, const SDL_FRect* dst_rect) const
	{
		if (!is_valid()) return;

		if (!SDL_RenderTexture(memory->renderer, item->second, src_rect, dst_rect))
			LOG_ERROR(Logging::Exceptions::SDLExceptions::Render::SDL_Render_RenderTexture_Failed(get_name()));
	}
	void Memory::Item::clear() const
	{
		if (!is_valid()) return;

		SDL_Texture* const current_target = SDL_GetRenderTarget(memory->renderer);
		SDL_SetRenderTarget(memory->renderer, item->second);
		SDL_RenderClear(memory->renderer);
		SDL_SetRenderTarget(memory->renderer, current_target);
	}
	Memory::Item::Item(const std::string& name, const Memory* memory) : memory(memory), item(memory->find(name).item) {
	}
	Memory::Item::Item(CONTAINER::const_iterator item_in_memory, const Memory* memory) : memory(memory), item(std::move(item_in_memory)) {
	}
}
