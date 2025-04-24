#include "structures/render/layer.h" // Header
#include "config.h"
#include "utilities.hpp"

using namespace Utilities::Math::FPoint;

namespace Structures::Render::Layers
{
	//! Layer
	// ::Camera
	uint8_t Layer::Camera::get_alpha() const { return color.a; }
	void Layer::Camera::set_alpha(const uint8_t& value) { color.a = value; }
	void Layer::Camera::move_x(const float& dx) { render_pos.x += dx; }
	void Layer::Camera::move_y(const float& dy) { render_pos.y += dy; }
	Layer::Camera::Camera(const OriginPoint& origin)
	{
		origin_point = origin; // góc trái
		render_pos = { .x = ::Config::GameConfig::Video::Camera::DEFAULT_POS_X, .y = ::Config::GameConfig::Video::Camera::DEFAULT_POS_Y };
	}

	SDL_FPoint Layer::Camera::get_camera_pos() const { return render_pos; }
	SDL_FPoint Layer::Camera::get_camera_size(const bool after_scale) const
	{
		return after_scale
			? SDL_FPoint{
				::Config::GameConfig::Video::Camera::DEFAULT_SIZE_WIDTH * scale.x,
				::Config::GameConfig::Video::Camera::DEFAULT_SIZE_HEIGHT * scale.y
		}
		: SDL_FPoint{ ::Config::GameConfig::Video::Camera::DEFAULT_SIZE_WIDTH, ::Config::GameConfig::Video::Camera::DEFAULT_SIZE_HEIGHT };
	}
	SDL_FPoint Layer::Camera::get_camera_object_offset() const
	{
		return -origin_point.convert_pos_to_origin(render_pos, { 0, 0 });
	}
	// ::RenderBuffer
	void Layer::RenderBuffer::remove_collection(Item& item)
	{
		item.remove();
	}
	// ::RenderBuffer::Item
	Layer::RenderBuffer::Item Layer::RenderBuffer::add_collection(const Objects::Collection* collection)
	{
		return { this, insert(end(), collection) };
	}
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
			collection_ptr->render(camera.get_camera_object_offset());
	}
	Layer::Layer(const SDL_FPoint& origin_pos_in_percent)
		: render_buffer(this),
		  camera({
			  origin_pos_in_percent * SDL_FPoint{
				  Config::GameConfig::Video::LOGICAL_WIDTH, Config::GameConfig::Video::LOGICAL_HEIGHT
			  }
		  })
	{
		
	}
	void Layer::reset(const bool to_initial_state)
	{
		render_buffer.clear();
		if (to_initial_state)
		{
			camera = Camera();
		}
	}
}