#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

namespace Work::Core
{
	namespace Init
	{
		void system(bool debug = false);
		void window(SDL_Window*& window);
		void renderer(SDL_Window* window, SDL_Renderer*& renderer);
	}

	namespace CleanUp
	{
		void system();
		void window(SDL_Window* window);
		void renderer(SDL_Renderer* renderer);
	}
}