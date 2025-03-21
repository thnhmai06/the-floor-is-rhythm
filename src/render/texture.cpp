#include "render/texture.h" // Header
#include <SDL3_image/SDL_image.h>
#include "logging.h"
#include "expections/sdl.h"

Texture::Texture(SDL_Renderer* renderer, const char* skin_file, const float size_x, const float size_y)
{
	this->renderer = renderer;
	this->size_x = size_x;
	this->size_y = size_y;
	texture = IMG_LoadTexture(renderer, skin_file);
	if (!texture) 
		THROW_ERROR(SDL_Exceptions::Texture::SDL_CouldntCreateTexture(skin_file));
}

void Texture::render(const float where_x, const float where_y) const
{
	const SDL_FRect dst = { where_x, where_y, size_x, size_y };
	if (!SDL_RenderTexture(renderer, texture, nullptr, &dst))
		THROW_ERROR(SDL_Exceptions::Texture::SDL_CouldntRenderTexture());
}

void Texture::destroy() const
{
	SDL_DestroyTexture(texture);
}