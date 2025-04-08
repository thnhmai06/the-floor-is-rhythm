#pragma once
#include "structures/screens/screen.h"
#include "parser/tfir/beatmap.h"

namespace Structures::Render::Screen::Gameplay
{
	struct PlayingScreen : private Screens::Screen
	{
		struct Render : Screen::Render
		{
			struct Components
			{
				Screen::Render::Item map_set;
				Screen::Render::Item cursor;

				Components(RenderObjects::RenderObjectStorage* storage,
					Layers::Layer* playground_layer, Layers::Layer* cursor_layer);
			} components;
			Template::Game::Direction::Direction current_direction = Template::Game::Direction::Direction::RIGHT;

			Render(Layers::Layer& playground_layer,
				Layers::Layer& cursor_layer,
				const Textures::TextureMemory& skin,
				const BeatmapFile& beatmap);
		} render;

		explicit PlayingScreen(const BeatmapFile& beatmap);
	};
}
