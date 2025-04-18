#include "structures/screens/screen.h" // Header

namespace Structures::Screens
{
	// ::Item
	void Screen::Render::Item::remove()
	{
		// render
		render_item.remove();

		// storage
		if (storage_item != storage->end())
		{
			storage->erase(storage_item);
			storage_item = storage->end();
		}
	}
	Screen::Render::Item::Item(Structures::Render::Objects::Storage* storage, Structures::Render::Layers::Layer::RenderBuffer* render_buffer) :
		storage(storage), storage_item(storage->end()), render_item(render_buffer)
	{
	}
}