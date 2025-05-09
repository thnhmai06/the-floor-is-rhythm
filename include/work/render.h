#pragma once
#include <SDL3/SDL_render.h>

namespace Work::Render
{
	inline SDL_Renderer* renderer = nullptr;

	void init(SDL_Window* window);
	void clean();
	int32_t work();
}