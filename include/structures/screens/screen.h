#pragma once
#include "structures/render/layers/objects/collection.h"
#include "structures/render/layers/layer.h"

namespace Structures::Screens
{
	using Render::RenderObjects::RenderObjectStorage;
	using Render::Layers::Layer;

	struct Screen
	{
		struct Render
		{
			RenderObjectStorage storage;

			struct Item
			{
				RenderObjectStorage* storage;
				RenderObjectStorage::iterator storage_item;
				Layer::RenderBuffer::Item render_item;

				void remove();

				Item(RenderObjectStorage* storage, Layer::RenderBuffer* render_buffer);
			};
		};
	};
}