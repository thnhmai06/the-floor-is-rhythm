#pragma once
#include "parser/tfir/beatmap.h"
#include "structures/render/collection.h"
#include "structures/render/layer.h"

namespace Structures::Screens
{
	using Render::RenderObjects::RenderObjectStorage;

	struct Screen
	{
		RenderObjectStorage storage;

		struct ScreenItem
		{
			RenderObjectStorage::iterator storage_item;
			Render::Layers::Layer::RenderBufferItem render_item;
		};
	};

	struct PlayingScreen : private Screen
	{
		ScreenItem map_set;

		PlayingScreen(
			Render::Layers::Layer& playground,
			const Render::Textures::TextureMemory& beatmap_skin, 
			const BeatmapFile& beatmap);
	};
}
