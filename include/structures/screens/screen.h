#pragma once
#include "structures/render/layers/objects/collection.h"
#include "structures/render/layers/layer.h"

namespace Structures::Screens
{
	using Render::Objects::Storage;
	using Render::Layers::Layer;

	struct Screen
	{
		struct Render
		{
			Storage storage;

			struct Item
			{
				Storage* storage;
				Storage::iterator storage_item;
				Layer::RenderBuffer::Item render_item;

				void remove();

				Item(Storage* storage, Layer::RenderBuffer* render_buffer);
			};
		};
	};
}