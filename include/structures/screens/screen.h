#pragma once
#include "structures/render/layers/objects/collection.h"
#include "structures/render/layers/layer.h"

namespace Structures::Screens
{
	struct Screen
	{
		struct Render
		{
			Structures::Render::Objects::Storage storage;

			struct Item
			{
				Structures::Render::Objects::Storage* storage;
				Structures::Render::Objects::Storage::iterator storage_item;
				Structures::Render::Layers::Layer::RenderBuffer::Item render_item;

				void remove();

				Item(Structures::Render::Objects::Storage* storage, Structures::Render::Layers::Layer::RenderBuffer* render_buffer);
			};
		};
	};
}