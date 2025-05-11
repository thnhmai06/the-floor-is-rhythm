#pragma once
#include <memory>
#include "structures/render/layer.h"
#include "structures/render/layer/storyboard.h"

namespace Core::Resources
{
	namespace Layers
	{
		using namespace Structures::Render::Layer;

		inline std::unique_ptr<Layer> normal_background; // background bình thường
		inline std::unique_ptr<StoryboardLayer> storyboard;
		inline std::unique_ptr<Layer> playground;
		inline std::unique_ptr<Layer> hud;
		inline std::unique_ptr<Layer> static_hud;
		inline std::unique_ptr<Layer> foreground;

		inline void init_all(SDL_Renderer* renderer)
		{
			normal_background = std::make_unique<Layer>();
			storyboard = std::make_unique<StoryboardLayer>(renderer);
			playground = std::make_unique<Layer>(SDL_FPoint{ 0.5, 0.5 }); // giữa màn hình
			hud = std::make_unique<Layer>();
			static_hud = std::make_unique<Layer>(); // giữa màn hình
			foreground = std::make_unique<Layer>();
		}
		inline void render_all()
		{
			if (normal_background) normal_background->render();
			if (storyboard) storyboard->render();
			if (playground) playground->render();
			if (hud) hud->render();
			if (static_hud) static_hud->render();
			if (foreground) foreground->render();
		}
		inline void clear_all()
		{
			if (normal_background) normal_background->clear();
			if (storyboard) storyboard->clear();
			if (playground) playground->clear();
			if (hud) hud->clear();
			if (static_hud) static_hud->clear();
			if (foreground) foreground->clear();
		}
	}
}
