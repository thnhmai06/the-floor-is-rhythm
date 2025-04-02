#pragma once
#include "render/layer.h"

int32_t render(SDL_Window* window);

namespace WorkingLayers
{
	inline std::unique_ptr<Layers::Layer> background;
	inline std::unique_ptr<Layers::PlaygoundLayer> playground;
	inline std::unique_ptr<Layers::Layer> cursor;
	inline std::unique_ptr<Layers::Layer> hud;

	static void init_all(SDL_Renderer* renderer)
	{
		if (!background) background = std::make_unique<Layers::Layer>(renderer);
		if (!playground) playground = std::make_unique<Layers::PlaygoundLayer>(renderer);
		if (!cursor) cursor = std::make_unique<Layers::Layer>(renderer);
		if (!hud) hud = std::make_unique<Layers::Layer>(renderer);
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