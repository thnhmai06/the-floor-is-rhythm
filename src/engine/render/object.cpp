#include "engine/render/object.h" // Header
#include "logging/logger.h"
#include "utilities.h"
#include "logging/exceptions.h"

namespace Engine::Render::Object
{
	//! Object
	SDL_FRect Object::get_sdl_src_rect() const
	{
		const auto [src_w, src_h] = src.get_size();
		return { src_w * src_rect_in_percent.x, src_h * src_rect_in_percent.y, src_w * src_rect_in_percent.w, src_h * src_rect_in_percent.h };
	}
	SDL_FRect Object::get_sdl_dst_rect() const
	{
		return config.get_sdl_dst_rect(Utilities::Render::get_size_from_rect(get_sdl_src_rect()));
	}
	void Object::set_render_size(const SDL_FPoint& size)
	{
		config.set_render_size(size, Utilities::Render::get_size_from_rect(get_sdl_src_rect()));
	}
	void Object::set_render_size(const float& value)
	{
		set_render_size({ value, value });
	}
	OriginPoint Object::translate_origin_type_to_point(const OriginType& origin_type, const bool based_on_render_size) const
	{
		if (!based_on_render_size)
		{
			const auto src_size = Utilities::Render::get_size_from_rect(get_sdl_src_rect());
			return OriginPoint::translate_origin_type_to_point(origin_type, src_size);
		}
		const auto render_size = Utilities::Render::get_size_from_rect(get_sdl_dst_rect());
		return OriginPoint::translate_origin_type_to_point(origin_type, render_size);
	}
	void Object::update_origin_point(const OriginPoint& custom_origin, const bool move_render_pos, const bool based_on_render_size)
	{
		const auto previous_origin = config.get_origin_point();
		config.set_origin_point(custom_origin, based_on_render_size);
		if (move_render_pos)
			config.render_pos = config.get_origin_point().convert_pos_from_origin(config.render_pos, previous_origin);
	}
	void Object::update_origin_point(const OriginType& origin_type, const bool move_render_pos)
	{
		update_origin_point(translate_origin_type_to_point(origin_type, false), move_render_pos, false);
	}
	Object::Object(
		Memory::Item texture,
		const OriginType& origin_type,
		const SDL_FPoint& render_pos) :
		src(std::move(texture))
	{
		update_origin_point(origin_type, false);
		config.render_pos = render_pos;
	}
	Object::Object(
		Memory::Item texture,
		const OriginPoint& custom_origin,
		const SDL_FPoint& render_pos) :
		src(std::move(texture)), config(render_pos, custom_origin)
	{
	}

	//! Collection
	void Collection::for_each_item(
		const std::function<bool(Container::reference& item)>& function,
		const bool no_duplicate)
	{
		std::unordered_set<void*> completed{};

		if (range.empty())
		{
			auto itr = data.begin();
			while (itr != data.end())
			{
				if (!function(*itr))
					itr = data.erase(itr);
				else ++itr;
			}
		}
		else for (auto& [from, to] : range)
		{
			static constexpr auto get_ptr = [](const Container::value_type& item) -> void*
			{
				if (std::holds_alternative<std::shared_ptr<Object>>(item))
					return std::get<std::shared_ptr<Object>>(item).get();
				if (std::holds_alternative<std::shared_ptr<Collection>>(item))
					return std::get<std::shared_ptr<Collection>>(item).get();
				return nullptr;
			};

			auto itr = from;
			while (itr != data.end())
			{
				const auto ptr = get_ptr(*itr);
				if (no_duplicate && completed.contains(ptr)) continue;
				function(*itr);
				completed.insert(ptr);
				if (itr == to) break;
			}
		}
	}
	//! Buffer
	Buffer::Item Buffer::add(const std::weak_ptr<Object>& object)
	{
		return { this, data.insert(data.end(), object) };
	}
	Buffer::Item Buffer::add(const std::weak_ptr<Collection>& collection)
	{
		return { this, data.insert(data.end(), collection) };
	}
	void Buffer::remove(Item& item) { item.destroy(); }
	void Buffer::for_each_item(
		const std::function<void(std::shared_ptr<Object>& object)>& object_function,
		const std::function<void(std::shared_ptr<Collection>& collection)>& collection_function)
	{
		if (data.empty()) return;

		auto itr = data.begin();
		while (itr != data.end())
		{
			if (std::holds_alternative<std::weak_ptr<Object>>(*itr))
			{
				auto& weak = std::get<std::weak_ptr<Object>>(*itr);
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
			else if (std::holds_alternative<std::weak_ptr<Collection>>(*itr))
			{
				auto& weak = std::get<std::weak_ptr<Collection>>(*itr);
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
	void Buffer::Item::destroy()
	{
		if (parent && item != parent->data.end())
		{
			parent->data.erase(item);
			item = parent->data.end();
		}
	}
	bool Buffer::Item::is_valid() const
	{
		if (!(parent && item != parent->data.end())) return false;
		if (std::holds_alternative<std::weak_ptr<Object>>(*item))
		{
			if (const auto& weak = std::get<std::weak_ptr<Object>>(*item);
				!Utilities::Pointer::check_weak(weak)) return false;
		}
		else if (std::holds_alternative<std::weak_ptr<Collection>>(*item))
		{
			if (const auto& weak = std::get<std::weak_ptr<Collection>>(*item);
				!Utilities::Pointer::check_weak(weak)) return false;
		}
		return true;
	}
	Buffer::Item::Item(Buffer* render_buffer) :
		parent(render_buffer)
	{
	}
	Buffer::Item::Item(Buffer* render_buffer, Container::iterator item) :
		parent(render_buffer), item(std::move(item))
	{
	}
}
