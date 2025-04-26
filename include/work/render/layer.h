#pragma once
#include <memory>
#include "structures/render/layer.h"
#include "structures/render/layer/storyboard.h"

namespace Work::Render::Layers
{
	using namespace Structures::Render::Layer;

	inline std::unique_ptr<Layer> normal; // background bình thường
	inline std::unique_ptr<StoryboardLayer> storyboard;
	inline std::unique_ptr<Layer> playground;
	inline std::unique_ptr<Layer> static_hud;
	inline std::unique_ptr<Layer> hud;

	inline void init_all(SDL_Renderer* renderer)
	{
		normal = std::make_unique<Layer>();
		storyboard = std::make_unique<StoryboardLayer>(renderer);
		playground = std::make_unique<Layer>(SDL_FPoint{ 0.5, 0.5 }); // giữa màn hình
		static_hud = std::make_unique<Layer>(); // giữa màn hình
		hud = std::make_unique<Layer>();
	}
	inline void render_all()
	{
		if (normal) normal->render();
		if (storyboard) storyboard->render();
		if (playground) playground->render();
		if (static_hud) static_hud->render();
		if (hud) hud->render();
	}
	inline void reset_all(const bool to_initial_state = false)
	{
		if (normal) normal->reset(to_initial_state);
		if (storyboard) storyboard->reset();
		if (playground) playground->reset(to_initial_state);
		if (static_hud) static_hud->reset(to_initial_state);
		if (hud) hud->reset(to_initial_state);
	}
}
