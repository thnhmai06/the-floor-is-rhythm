#include "render/texture.h" // Header
#include <ranges>
#include "logging.h"
#include "exceptions.h"

//! Texture
// ::TextureMemory
SDL_Texture* Texture::TextureMemory::load(const char* file_path, const std::string& name)
{
	SDL_Texture* texture = IMG_LoadTexture(this->renderer, file_path);
	if (!texture)
		THROW_ERROR(SDL_Exceptions::Texture::SDL_LoadTexture_Failed(file_path));

	// Nếu dùng STB_Image
	/*if (!texture)
		THROW_ERROR(SDL_Exceptions::Texture::IMG_LoadTexture_Failed(file_path));

	int32_t w, h, channels;
	unsigned char* image = stbi_load(file_path, &w, &h, &channels, desired_channels);
	if (!image)
		THROW_ERROR(STBI_Exceptions::Image::STBI_LoadImage_Failed(file_path));
	SDL_Texture* texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w, h);

	if (!SDL_UpdateTexture(texture, nullptr, image, w * desired_channels))
	{
		stbi_image_free(image);
		THROW_ERROR(SDL_Exceptions::Texture::SDL_UpdateTexture_Failed(file_path));
	}
	stbi_image_free(image);*/

	return load(texture, name);
}
SDL_Texture* Texture::TextureMemory::load(SDL_Texture* texture, const std::string& name)
{
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	insert_or_assign(name, texture);
	return texture;
}
bool Texture::TextureMemory::render(const std::string& name, const TextureConfig& config) const
{
	if (const auto it = find(name); it != end())
	{
		SDL_SetTextureAlphaMod(it->second, config.alpha);

		if (config.rotation)
			return SDL_RenderTextureRotated(renderer, it->second, config.src_rect.get(), config.dst_rect.get(),
				config.rotation->angle, config.rotation->center.get(), config.rotation->flip);
		return SDL_RenderTexture(renderer, it->second, config.src_rect.get(), config.dst_rect.get());
	}
	return false;
}
void Texture::TextureMemory::free(const std::string& name)
{
	const auto it = find(name);
	if (it != end())
	{
		SDL_DestroyTexture(it->second);
		erase(it);
	}
}
void Texture::TextureMemory::free_all()
{
	for (const auto& texture : *this | std::views::values)
		SDL_DestroyTexture(texture);
	std::unordered_map<std::string, SDL_Texture*>::clear();
}

//! RenderObjects
// ::RenderObject
void RenderObjects::RenderObject::render(const Texture::TextureMemory& memory) const
{
	if (!memory.render(*name, config))
		LOG_ERROR(SDL_Exceptions::Texture::SDL_RenderTexture_Failed(*name));

}