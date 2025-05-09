#include "structures/render/layer.h" // Header
#include "config.h"
#include "utilities.hpp"
#include "logging/logger.h"
#include "logging/exceptions.h"

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

	const SDL_FPoint& Layer::Camera::get_camera_origin() const { return origin_point; }
	SDL_FPoint& Layer::Camera::get_camera_origin() { return origin_point; }
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
	Layer::Buffer::Item Layer::Buffer::add(const std::weak_ptr<Object::Object>& object)
	{
		return { this, data.insert(data.end(), object) };
	}
	Layer::Buffer::Item Layer::Buffer::add(const std::weak_ptr<Object::Collection>& collection)
	{
		return { this, data.insert(data.end(), collection) };
	}


	void Layer::Buffer::remove(Item& item) { item.destroy(); }
	void Layer::Buffer::for_each_item(
		const std::function<void(std::shared_ptr<Object::Object>& object)>& object_function,
		const std::function<void(std::shared_ptr<Object::Collection>& collection)>& collection_function)
	{
		if (data.empty()) return;

		auto itr = data.begin();
		while (itr != data.end())
		{
			if (std::holds_alternative<std::weak_ptr<Object::Object>>(*itr))
			{
				auto& weak = std::get<std::weak_ptr<Object::Object>>(*itr);
				if (weak.expired())
				{
					itr = data.erase(itr);
					continue;
				}
				auto shared = weak.lock();
				if (!shared)
				{
					itr = data.erase(itr);
					continue;
				}
				object_function(shared);
				++itr;
			}
			else if (std::holds_alternative<std::weak_ptr<Object::Collection>>(*itr))
			{
				auto& weak = std::get<std::weak_ptr<Object::Collection>>(*itr);
				if (weak.expired())
				{
					itr = data.erase(itr);
					continue;
				}
				auto shared = weak.lock();
				if (!shared)
				{
					itr = data.erase(itr);
					continue;
				}
				collection_function(shared);
				++itr;
			}
		}
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
	bool Layer::Buffer::Item::is_valid() const
	{
		if (!(parent && item != parent->data.end())) return false;
		if (std::holds_alternative<std::weak_ptr<Object::Object>>(*item))
		{
			if (const auto& weak = std::get<std::weak_ptr<Object::Object>>(*item);
				weak.expired() || !weak.lock()) return false;
		}
		else if (std::holds_alternative<std::weak_ptr<Object::Collection>>(*item))
		{
			if (const auto& weak = std::get<std::weak_ptr<Object::Collection>>(*item);
				weak.expired() || !weak.lock()) return false;
		}
		return true;
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
	void Layer::render_object(std::shared_ptr<Object::Object>& object, const SDL_FPoint& offset)
	{
		if (!visible || object->config.color.a == 0) return;
		if (!object->src.is_valid()) return;

		object->on_before_render();
		object->config.render_pos += offset;
		put_object_into_layer(object);
		const auto& sdl_texture = object->src.item->second;
		const auto src_rect = object->get_sdl_src_rect();
		auto dst_rect = object->get_sdl_dst_rect();
		const auto camera_offset = camera.get_camera_object_offset();
		dst_rect.x += camera_offset.x; dst_rect.y += camera_offset.y;
		const SDL_FPoint render_origin_point = object->config.get_origin_point(true);

		// https://stackoverflow.com/questions/24969783/is-it-safe-acceptable-to-call-sdl-settexturecolormod-every-frame-multiple-times
		SDL_SetTextureBlendMode(sdl_texture, object->config.blend_mode);
		SDL_SetTextureAlphaMod(sdl_texture, object->config.color.a);
		SDL_SetTextureColorMod(sdl_texture, object->config.color.r, object->config.color.g, object->config.color.b);
		if (!SDL_RenderTextureRotated(object->src.memory->renderer, sdl_texture, &src_rect,
			&dst_rect, object->config.angle, &render_origin_point, object->config.flip_mode))
			LOG_ERROR(Logging::Exceptions::SDLExceptions::Texture::SDL_Texture_Render_Failed(object->src.get_name()));
		put_object_out_layer(object);
		object->config.render_pos -= offset;
		object->on_after_render();
	}
	void Layer::render_collection(std::shared_ptr<Object::Collection>& collection, const SDL_FPoint& bonus_offset)
	{
		if (!collection->visible) return;

		collection->on_before_render();
		const auto total_offset = collection->offset + bonus_offset;
		collection->for_each_item([&](const size_t& index)
			{
				if (auto& item = collection->data[index];
					std::holds_alternative<std::shared_ptr<Object::Object>>(item))
				{
					auto& item_object = std::get<std::shared_ptr<Object::Object>>(item);
					render_object(item_object, total_offset);
				}
				else if (std::holds_alternative<std::shared_ptr<Object::Collection>>(item))
				{
					auto& item_collection = std::get<std::shared_ptr<Object::Collection>>(item);
					render_collection(item_collection, total_offset);
				}
			}, true);
		collection->on_after_render();
	}
	void Layer::render()
	{
		if (!visible) return;
		render_buffer.for_each_item(
			[&](std::shared_ptr<Object::Object>& item) { render_object(item); },
			[&](std::shared_ptr<Object::Collection>& item) { render_collection(item); });
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
	void Layer::clear()
	{
		render_buffer.data.clear();
	}

	//! TextureLayer
	void TextureLayer::put_object_into_layer(std::shared_ptr<Object::Object>& object)
	{
		const auto pixel = target_texture.get_size() / grid_size.value();
		object->config.render_pos *= pixel;
		object->config.scale *= pixel;
		Layer::put_object_into_layer(object);
	}
	void TextureLayer::put_object_out_layer(std::shared_ptr<Object::Object>& object)
	{
		const auto pixel = target_texture.get_size() / grid_size.value();
		object->config.render_pos /= pixel;
		object->config.scale /= pixel;
		Layer::put_object_out_layer(object);
	}
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
	void TextureLayer::clear()
	{
		Layer::clear();
		target_texture.clear();
	}
	TextureLayer::TextureLayer(Memory::Item texture,
		const SDL_FPoint& camera_origin_in_percent)
		: Layer(camera_origin_in_percent), target_texture(std::move(texture))
	{
	}
}