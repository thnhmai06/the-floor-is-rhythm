#pragma once
#include "render/layer.h"

int32_t render(SDL_Window* window);

namespace Current_Layers
{
	inline std::unique_ptr<Layer> background;
	inline std::unique_ptr<Layer> playground;
	inline std::unique_ptr<Layer> cursor;
	inline std::unique_ptr<Layer> hud;

	static void init_all(SDL_Renderer* renderer)
	{
		if (!background) background = std::make_unique<Layer>(renderer);
		if (!playground) playground = std::make_unique<Layer>(renderer);
		if (!cursor) cursor = std::make_unique<Layer>(renderer);
		if (!hud) hud = std::make_unique<Layer>(renderer);
	}
	static void render_all()
	{
		if (background) background->render();
		if (playground) playground->render();
		if (cursor) cursor->render();
		if (hud) hud->render();
	}
	static void clear_all(const bool to_initial_state = false)
	{
		if (background) background->clear(to_initial_state);
		if (playground) playground->clear(to_initial_state);
		if (cursor) cursor->clear(to_initial_state);
		if (hud) hud->clear(to_initial_state);
	}
}