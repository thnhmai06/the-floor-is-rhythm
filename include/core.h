#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

namespace Core
{
	namespace Init
	{
		void window(SDL_Window*& window);
		void renderer(SDL_Window* window, SDL_Renderer*& renderer);
	}

	namespace CleanUp
	{
		void window(SDL_Window* window);
		void renderer(SDL_Renderer* renderer);
	}
}