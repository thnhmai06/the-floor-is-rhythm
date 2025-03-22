#include "render/texture.h" // Header
#include <ranges>
#include "logging.h"
#include "exceptions.h"

SDL_Texture* TextureMemory::load(const char* file_path, const std::string& name)
{
	// Vì SDL_image không hỗ trợ load jpg, nên cần thông qua stb_image
	/*SDL_Texture* texture = IMG_LoadTexture(this->renderer, file_path);
	if (!texture)
		THROW_ERROR(SDL_Exceptions::Texture::IMG_LoadTexture_Failed(file_path));*/

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
		
	stbi_image_free(image);
	return load(texture, name);
}
SDL_Texture* TextureMemory::load(SDL_Texture* texture, const std::string& name)
{
	insert_or_assign(name, texture);
	return texture;
}

bool TextureMemory::render(const std::string& name, const TextureRenderConfig& config) const
{
	if (auto it = find(name); it != end())
	{
		if (config.rotation.has_value())
			return SDL_RenderTextureRotated(renderer, it->second, config.src_rect, config.dst_rect,
				config.rotation->angle, config.rotation->center, config.rotation->flip);
		return SDL_RenderTexture(renderer, it->second, config.src_rect, config.dst_rect);
	}
	return false;
}

void TextureMemory::free(const std::string& name)
{
	auto it = find(name);
	if (it != end())
	{
		SDL_DestroyTexture(it->second);
		erase(it);
	}
}

void TextureMemory::free_all()
{
	for (auto& texture : *this | std::views::values)
		SDL_DestroyTexture(texture);
	clear();
}
