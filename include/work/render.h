#pragma once
#include "structures/render/layer.h"

int32_t render(SDL_Window* window);

namespace WorkingComponents::Render
{
	namespace TextureMemory
	{
		using Structures::Render::Textures::TextureMemory;

		inline std::unique_ptr<TextureMemory> skin;
		static void init_all(SDL_Renderer* renderer)
		{
			if (!skin) skin = std::make_unique<TextureMemory>(renderer);
		}
		static void clear_all()
		{
			if (skin) skin->clear();
		}
	}

	namespace Layers
	{
		using namespace Structures::Render::Layers;

		inline std::unique_ptr<Layer> background;
		inline std::unique_ptr<Layer> playground;
		inline std::unique_ptr<Layer> cursor;
		inline std::unique_ptr<Layer> hud;

		static void init_all()
		{
			background = std::make_unique<Layer>();
			playground = std::make_unique<Layer>();
			cursor = std::make_unique<Layer>();
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
}