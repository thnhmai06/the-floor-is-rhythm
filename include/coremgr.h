#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

namespace Init
{
	SDL_Window* window();
	SDL_Renderer* renderer(SDL_Window* window);
}

namespace CleanUp
{
	void window(SDL_Window* window);
	void renderer(SDL_Renderer* renderer);
}