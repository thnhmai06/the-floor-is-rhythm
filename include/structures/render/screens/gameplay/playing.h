#pragma once
#include "structures/render/screens/screen.h"
#include "parser/tfir/beatmap.h"

namespace Structures::Render::Screen::Gameplay
{
	struct PlayingScreen : private Screens::Screen
	{
		struct Components
		{
			Item map_set;
			Item cursor;

			Components(RenderObjects::RenderObjectStorage* storage,
				Layers::Layer* playground_layer, Layers::Layer* cursor_layer);
		} components;
		Template::Game::Direction::Direction current_direction = Template::Game::Direction::Direction::RIGHT;

		PlayingScreen(
			Layers::Layer& playground_layer,
			Layers::Layer& cursor_layer,
			const Textures::TextureMemory& skin,
			const BeatmapFile& beatmap);
	};
}
