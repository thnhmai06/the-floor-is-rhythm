#include "structures/screens/screen.h" // Header

using Structures::Screens::Screen;
// :: Item
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
Screen::Render::Item::Item(RenderObjectStorage* storage, Layer::RenderBuffer* render_buffer) :
	storage(storage), storage_item(storage->end()), render_item(render_buffer)
{
}