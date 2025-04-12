#pragma once
#include <memory>
#include "structures/render/layers/layer.h"

namespace Work::Render::Layers
{
	using namespace Structures::Render::Layers;

	inline std::unique_ptr<Layer> background;
	inline std::unique_ptr<Layer> playground;
	inline std::unique_ptr<Layer> cursor;
	inline std::unique_ptr<Layer> hud;

	static void init_all()
	{
		background = std::make_unique<Layer>();
		playground = std::make_unique<Layer>(SDL_FPoint{ 0.5, 0.5 }); // giữa màn hình
		cursor = std::make_unique<Layer>(SDL_FPoint{ 0.5, 0.5 }); // giữa màn hình
		hud = std::make_unique<Layer>();
	}
	static void render_all()
	{
		if (background) background->render();
		if (playground) playground->render();
		if (cursor) cursor->render();
		if (hud) hud->render();
	}
	static void reset_all(const bool to_initial_state = false)
	{
		if (background) background->reset(to_initial_state);
		if (playground) playground->reset(to_initial_state);
		if (cursor) cursor->reset(to_initial_state);
		if (hud) hud->reset(to_initial_state);
	}
}
