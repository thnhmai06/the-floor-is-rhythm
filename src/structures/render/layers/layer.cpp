#include "structures/render/layers/layer.h" // Header
#include "config.h"
#include "utilities.h"

using namespace Utilities::Math::FPoint;

// Structures::Render::Layers::Layer
using Structures::Render::Layers::Layer;
// ::LayerCamera
uint8_t Layer::LayerCamera::LayerCamera::get_alpha() const { return alpha; }
void Layer::LayerCamera::LayerCamera::set_alpha(const uint8_t& value) { alpha = value; }
void Layer::LayerCamera::LayerCamera::move_x(const float& dx) { render_pos.x += dx; }
void Layer::LayerCamera::LayerCamera::move_y(const float& dy) { render_pos.y += dy; }
SDL_FPoint Layer::LayerCamera::LayerCamera::get_camera_pos() const { return render_pos; }
SDL_FPoint Layer::LayerCamera::LayerCamera::get_camera_size(const bool after_scale) const
{
	return after_scale
		? SDL_FPoint{
			GameConfig::Video::Camera::DEFAULT_SIZE_WIDTH * scale.x,
			GameConfig::Video::Camera::DEFAULT_SIZE_HEIGHT * scale.y
	}
	: SDL_FPoint{ GameConfig::Video::Camera::DEFAULT_SIZE_WIDTH, GameConfig::Video::Camera::DEFAULT_SIZE_HEIGHT };
}
SDL_FPoint Layer::LayerCamera::get_object_offset() const
{
	return -origin_pos.convert_pos_to_origin(render_pos, { 0, 0 });
}
// ::RenderBuffer
Layer::RenderBuffer::Item Layer::RenderBuffer::add_collection(const RenderObjects::RenderObjectCollection* collection)
{
	return { this, insert(end(), collection) };
}
void Layer::RenderBuffer::remove_collection(Item& item)
{
	item.remove();
}
// ::RenderBuffer::Item
void Layer::RenderBuffer::Item::remove()
{
	if (render_buffer && item != render_buffer->end())
	{
		render_buffer->erase(item);
		item = render_buffer->end();
	}
}
Layer::RenderBuffer::Item::Item(RenderBuffer* render_buffer) :
	render_buffer(render_buffer), item(render_buffer->end())
{
}
Layer::RenderBuffer::Item::Item(RenderBuffer* render_buffer, iterator item) :
	render_buffer(render_buffer), item(std::move(item))
{
}
// ::
void Layer::render() const
{
	if (!visible || render_buffer.empty()) return;
	for (const auto& collection_ptr : render_buffer)
		collection_ptr->render(camera.get_object_offset());
}
Layer::Layer() : render_buffer(this) {}
void Layer::reset(const bool to_initial_state)
{
	render_buffer.clear();
	if (to_initial_state)
	{
		camera = LayerCamera();
	}
}