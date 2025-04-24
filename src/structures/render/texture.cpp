#include "structures/render/texture.h" // Header
#include <ranges>
#include <SDL3_image/SDL_image.h>
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "utilities.hpp"

namespace Structures::Render::Textures
{
	//! TextureMemory
	// ::
	TextureMemory::Item TextureMemory::load_texture(const char* file_path, const std::string& name, const bool override)
	{
		if (!override && contains(name)) return {};
		SDL_Texture* texture = IMG_LoadTexture(this->renderer, file_path);
		if (!texture)
			THROW_ERROR(Logging::Exceptions::SDLExceptions::Texture::SDL_Texture_Load_Failed(file_path));

		// Nếu dùng STB_Image
		/*if (!texture)
			THROW_ERROR(Logging::Exceptions::SDLExceptions::Texture::IMG_LoadTexture_Failed(file_path));

		int32_t w, h, channels;
		unsigned char* image = stbi_load(file_path, &w, &h, &channels, desired_channels);
		if (!image)
			THROW_ERROR(STBI_Logging::Exceptions::Image::STBI_LoadImage_Failed(file_path));
		SDL_Texture* texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w, h);

		if (!SDL_UpdateTexture(texture, nullptr, image, w * desired_channels))
		{
			stbi_image_free(image);
			THROW_ERROR(Logging::Exceptions::SDLExceptions::Texture::SDL_UpdateTexture_Failed(file_path));
		}
		stbi_image_free(image);*/

		return load_texture(texture, name, override);
	}
	TextureMemory::Item TextureMemory::load_texture(SDL_Texture* texture, const std::string& name, const bool override)
	{
		if (!override && contains(name)) return {};
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		free_texture(name); // Nếu không có sẽ không làm gì
		const auto item = insert_or_assign(name, texture).first;
		return { item, this };
	}
	TextureMemory::Item TextureMemory::find(const std::string& name) const
	{
		if (const auto it = BASE::find(name); it != end())
			return { it, this };
		return {};
	}
	SDL_FPoint TextureMemory::get_texture_size(const const_iterator& texture)
	{
		float width, height;
		SDL_GetTextureSize(texture->second, &width, &height);
		return { width, height };
	}
	SDL_FPoint TextureMemory::get_texture_size(const std::string& name) const
	{
		if (const auto it = BASE::find(name); it != end())
			return get_texture_size(it);
		return { 0, 0 };
	}
	TextureMemory::iterator TextureMemory::rename_texture(const std::string& old_name, const std::string& new_name)
	{
		if (const auto item = BASE::find(old_name); item != end())
		{
			auto new_location = insert_or_assign(new_name, item->second).first;
			erase(item);
			return new_location;
		}
		return end();
	}
	TextureMemory::iterator TextureMemory::move_texture(const std::string& name, TextureMemory* to_memory)
	{
		if (const auto current = BASE::find(name); current != end())
		{
			auto new_location = to_memory->insert_or_assign(name, current->second).first;
			erase(current);
			return new_location;
		}
		return end();
	}
	void TextureMemory::free_texture(const std::string& name)
	{
		if (const auto it = BASE::find(name); it != end())
		{
			if (it->second) SDL_DestroyTexture(it->second);
			erase(it);
		}
	}
	void TextureMemory::free_all()
	{
		for (const auto& texture : *this | std::views::values)
			SDL_DestroyTexture(texture);
		BASE::clear();
	}

	// ::Item
	const std::string& TextureMemory::Item::get_name() const { return item->first; }
	SDL_FPoint TextureMemory::Item::get_size() const { return memory->get_texture_size(get_name()); }
	void TextureMemory::Item::change_target(const std::string& new_target, const TextureMemory* new_memory)
	{
		if (new_memory) memory = new_memory;
		if (!new_target.empty())
			item = memory->find(new_target).item;
	}
	TextureMemory::Item::Item(const std::string& name, const TextureMemory* memory) : memory(memory), item(memory->find(name).item) {
	}
	TextureMemory::Item::Item(const_iterator item_in_memory, const TextureMemory* memory) : memory(memory), item(std::move(item_in_memory)) {
	}
}