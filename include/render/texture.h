#pragma once
#include <SDL3/SDL_render.h>

struct Texture
{
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	float size_x;
	float size_y;

	Texture(SDL_Renderer* renderer, const char* skin_file, float size_x, float size_y);
	void render(float where_x, float where_y) const;
	void destroy() const;
};
