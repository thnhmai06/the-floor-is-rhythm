#pragma once
#include <SDL3/SDL_render.h>

namespace RenderThread
{
	constexpr char name[] = "RenderThread";
	int Worker(void* data);

	SDL_Thread* start_thread(SDL_Renderer*& renderer);
}
