#pragma once
#include "render/layer.h"
#include "utilities.h"

int32_t render(SDL_Window* window);

namespace Current_Layers
{
	inline std::unique_ptr<Layer> background;
	inline std::unique_ptr<Layer> playground;
	inline std::unique_ptr<Layer> hud;

	static void init_all(SDL_Renderer* renderer)
	{
		if (!background) background = std::make_unique<Layer>(renderer);
		if (!playground) playground = std::make_unique<Layer>(renderer);
		if (!hud) hud = std::make_unique<Layer>(renderer);
	}
	static void render_all()
	{
		background->render();
		playground->render();
		hud->render();
	}
	static void free_all()
	{
		if (background) background->free();
		if (playground) playground->free();
		if (hud) hud->free();
	}
}