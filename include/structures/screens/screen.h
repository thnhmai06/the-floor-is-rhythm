#pragma once
#include "structures/render/collection.h"
#include "structures/render/layer.h"

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
				Structures::Render::Objects::Storage::iterator collection;
				Structures::Render::Layers::Layer::RenderBuffer::Item render_item;

				void remove();

				Item(Structures::Render::Objects::Storage* storage, Structures::Render::Layers::Layer::RenderBuffer* render_buffer);
			};
		};
	};
}