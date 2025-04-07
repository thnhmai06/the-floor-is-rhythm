#pragma once
#include "structures/render/layers/objects/collection.h"
#include "structures/render/layers/layer.h"

namespace Structures::Render::Screens
{
	using RenderObjects::RenderObjectStorage;

	struct Screen
	{
		RenderObjectStorage storage;

		struct Item
		{
			RenderObjectStorage* storage;
			RenderObjectStorage::iterator storage_item;
			Layers::Layer::RenderBuffer::Item render_item;

			void remove();

			Item(RenderObjectStorage* storage, Layers::Layer::RenderBuffer* render_buffer);
		};
	};
}
