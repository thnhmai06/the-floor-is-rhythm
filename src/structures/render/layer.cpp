#include "structures/render/layer.h" // Header
#include "config.h"
#include "utilities.hpp"

using namespace Utilities::Math::FPoint;

namespace Structures::Render::Layer
{
	//! Layer
	// ::Camera
	Layer::Camera::Camera(const OriginPoint& origin)
	{
		origin_point = origin;
		render_pos = { .x = ::Config::GameConfig::Render::Camera::DEFAULT_POS_X, .y = ::Config::GameConfig::Render::Camera::DEFAULT_POS_Y };
	}

	const SDL_FPoint& Layer::Camera::get_camera_origin() const
	{
		return origin_point;
	}
	SDL_FPoint& Layer::Camera::get_camera_origin()
	{
		return origin_point;
	}

	SDL_FPoint& Layer::Camera::get_camera_pos() { return render_pos; }
	const SDL_FPoint& Layer::Camera::get_camera_pos() const { return render_pos; }
	SDL_FPoint Layer::Camera::get_camera_size()
	{
		return SDL_FPoint{ ::Config::GameConfig::Render::Camera::DEFAULT_SIZE_WIDTH, ::Config::GameConfig::Render::Camera::DEFAULT_SIZE_HEIGHT };
	}
	SDL_FPoint Layer::Camera::get_camera_object_offset() const
	{
		return -origin_point.convert_pos_to_origin(render_pos, { 0, 0 });
	}
	// ::Buffer
	Layer::Buffer::Item Layer::Buffer::add(const std::weak_ptr<Object::Collection>& collection)
	{
		return { this, data.insert(data.end(), collection) };
	}
	void Layer::Buffer::destroy(Item& item)
	{
		item.destroy();
	}
	// ::Buffer::Item
	void Layer::Buffer::Item::destroy()
	{
		if (parent && item != parent->data.end())
		{
			parent->data.erase(item);
			item = parent->data.end();
		}
	}
	Layer::Buffer::Item::Item(Buffer* render_buffer) :
		parent(render_buffer), item(render_buffer->data.end())
	{
	}
	Layer::Buffer::Item::Item(Buffer* render_buffer, CONTAINER::iterator item) :
		parent(render_buffer), item(std::move(item))
	{
	}

	// ::
	void Layer::render()
	{
		if (!visible || render_buffer.data.empty()) return;
		auto itr = render_buffer.data.begin();
		while (itr != render_buffer.data.end())
		{
			if (itr->expired())
			{
				itr = render_buffer.data.erase(itr);
				continue;
			}

			const auto camera_offset = camera.get_camera_object_offset();
			const auto& collection = itr->lock();
			collection->render(camera_offset);
			++itr;
		}
	}
	Layer::Layer(const SDL_FPoint& camera_origin_in_percent)
		: render_buffer(this),
		camera({
			camera_origin_in_percent * SDL_FPoint{
				Config::GameConfig::Render::LOGICAL_WIDTH,
			  Config::GameConfig::Render::LOGICAL_HEIGHT
			}
			})
	{
	}
	void Layer::reset(const bool reset_camera)
	{
		render_buffer.data.clear();
		if (reset_camera)
			camera = Camera();
	}

	//! TextureLayer
	void TextureLayer::render_no_change_back_target(const bool clear)
	{
		if (!visible || render_buffer.data.empty()) return;

		SDL_SetRenderTarget(target_texture.memory->renderer, target_texture.item->second);
		if (clear) SDL_RenderClear(target_texture.memory->renderer);
		Layer::render();
	}
	void TextureLayer::render()
	{
		if (!visible || render_buffer.data.empty()) return;

		SDL_Texture* const current_target = SDL_GetRenderTarget(target_texture.memory->renderer);
		render_no_change_back_target();
		SDL_SetRenderTarget(target_texture.memory->renderer, current_target);
	}
	void TextureLayer::reset(const bool reset_camera)
	{
		Layer::reset(reset_camera);
		target_texture.clear();
	}
	TextureLayer::TextureLayer(Memory::Item texture,
		const SDL_FPoint& camera_origin_in_percent)
		: Layer(camera_origin_in_percent), target_texture(std::move(texture))
	{
	}
}