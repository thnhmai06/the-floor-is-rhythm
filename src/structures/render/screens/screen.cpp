#include "structures/screens/screen.h" // Header

using Structures::Render::Screens::Screen;
// :: Item
void Screen::Item::remove()
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
Screen::Item::Item(RenderObjectStorage* storage, Layers::Layer::RenderBuffer* render_buffer) :
	storage(storage), storage_item(storage->end()), render_item(render_buffer)
{
}