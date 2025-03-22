#include "render/texture.h" // Header
#include <ranges>
#include "logging.h"
#include "exceptions/sdl.h"

SDL_Texture* TextureMemory::load(const char* file_path, const std::string& name)
{
	SDL_Texture* texture = IMG_LoadTexture(this->renderer, file_path);
	if (!texture)
		THROW_ERROR(SDL_Exceptions::Texture::IMG_LoadTexture_Failed(file_path));

	insert_or_assign(name, texture);
	return texture;
}

SDL_Texture* TextureMemory::load(SDL_Texture* texture, const std::string& name)
{
	insert_or_assign(name, texture);
	return texture;
}

void TextureMemory::render(const std::string& name, const SDL_FRect* rect) const
{
	if (auto it = find(name); it != end())
		SDL_RenderTexture(this->renderer, it->second, nullptr, rect);
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
