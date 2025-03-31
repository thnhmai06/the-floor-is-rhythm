#include "render/texture.h" // Header
#include <ranges>
#include <SDL3_image/SDL_image.h>
#include "logging.h"
#include "exceptions.h"
#include "utilities.h"

// TextureMemory
SDL_Texture* TextureMemory::load(const char* file_path, const std::string& name)
{
	SDL_Texture* texture = IMG_LoadTexture(this->renderer, file_path);
	if (!texture)
		THROW_ERROR(SDLExceptions::Texture::SDL_Texture_Load_Failed(file_path));

	// Nếu dùng STB_Image
	/*if (!texture)
		THROW_ERROR(SDLExceptions::Texture::IMG_LoadTexture_Failed(file_path));

	int32_t w, h, channels;
	unsigned char* image = stbi_load(file_path, &w, &h, &channels, desired_channels);
	if (!image)
		THROW_ERROR(STBI_Exceptions::Image::STBI_LoadImage_Failed(file_path));
	SDL_Texture* texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w, h);

	if (!SDL_UpdateTexture(texture, nullptr, image, w * desired_channels))
	{
		stbi_image_free(image);
		THROW_ERROR(SDLExceptions::Texture::SDL_UpdateTexture_Failed(file_path));
	}
	stbi_image_free(image);*/

	return load(texture, name);
}
SDL_Texture* TextureMemory::load(SDL_Texture* texture, const std::string& name)
{
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	insert_or_assign(name, texture);
	return texture;
}
SDL_FPoint TextureMemory::get_texture_size(const const_iterator& texture)
{
	float width, height;
	SDL_GetTextureSize(texture->second, &width, &height);
	return { width, height };
}
SDL_FPoint TextureMemory::get_texture_size(const std::string& name) const
{
	if (const auto it = find(name); it != end())
		return get_texture_size(it);
	return { 0, 0 };
}
void TextureMemory::free_texture(const std::string& name)
{
	if (const auto it = find(name); it != end() && it->second)
	{
		SDL_DestroyTexture(it->second);
		erase(it);
	}
}
void TextureMemory::free_all()
{
	for (const auto& texture : *this | std::views::values)
		SDL_DestroyTexture(texture);
	std::unordered_map<std::string, SDL_Texture*>::clear();
}

// Texture
void TextureManager::free() { memory->free_texture(name); name = ""; memory = nullptr; }
SDL_FPoint TextureManager::get_size() const { return memory->get_texture_size(name); }
void TextureManager::rename(const std::string& new_name) const
{
	if (const auto item = memory->find(name); item != memory->end())
	{
		memory->insert_or_assign(new_name, item->second);
		memory->erase(item);
	}
}
void TextureManager::move(TextureMemory* to_memory)
{
	auto current = memory->find(name);
	to_memory[name] = std::move(current);
	memory->erase(current);
	memory = to_memory;
}
TextureManager::TextureManager(const TextureMemory::iterator& memory_item, TextureMemory* memory)
{
	name = memory_item->first;
	this->memory = memory;
}