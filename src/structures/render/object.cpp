#include "structures/render/object.h" // Header
#include "logging/logger.h"
#include "utilities.hpp"

namespace Structures::Render::Object
{
	//! Object
	// ::OriginPoint
	SDL_FPoint Object::Config::OriginPoint::convert_pos_from_origin(const SDL_FPoint& pos, const OriginPoint& from_origin) const
	{
		return { pos.x - from_origin.x + x , pos.y - from_origin.y + y };
	}
	SDL_FPoint Object::Config::OriginPoint::convert_pos_to_origin(const SDL_FPoint& pos, const OriginPoint& to_origin) const
	{
		return { pos.x - x + to_origin.x, pos.y - y + to_origin.y };
	}
	SDL_FRect Object::Config::OriginPoint::convert_rect_from_origin(const SDL_FRect& rect, const OriginPoint& to_origin) const
	{
		const auto [new_x, new_y] = convert_pos_from_origin({ .x = rect.x, .y = rect.y }, to_origin);
		return { new_x, new_y, rect.w, rect.h };
	}
	SDL_FRect Object::Config::OriginPoint::convert_rect_to_origin(const SDL_FRect& rect, const OriginPoint& to_origin) const
	{
		const auto [new_x, new_y] = convert_pos_to_origin({ .x = rect.x, .y = rect.y }, to_origin);
		return { new_x, new_y, rect.w, rect.h };
	}
	Object::Config::OriginPoint Object::Config::OriginPoint::translate_origin_type_to_point(
		const Types::Render::OriginType& origin_type, const SDL_FPoint& size)
	{
		const auto& [w, h] = size;
		switch (origin_type)
		{
		case Types::Render::OriginType::TopLeft:
		case Types::Render::OriginType::Custom:
			return { 0, 0 };
		case Types::Render::OriginType::BottomLeft:
			return { 0, h };
		case Types::Render::OriginType::BottomRight:
			return { w, h };
		case Types::Render::OriginType::TopRight:
			return { w, 0 };
		case Types::Render::OriginType::TopCentre:
			return { w / 2, 0 };
		case Types::Render::OriginType::BottomCentre:
			return { w / 2, h };
		case Types::Render::OriginType::CentreLeft:
			return { 0, h / 2 };
		case Types::Render::OriginType::CentreRight:
			return { w, h / 2 };
		case Types::Render::OriginType::Centre:
			return { w / 2, h / 2 };
		}
		return { 0, 0 };
	}
	Object::Config::OriginPoint::OriginPoint(
		const Types::Render::OriginType& origin_type, const SDL_FPoint& size)
		: OriginPoint(translate_origin_type_to_point(origin_type, size))
	{
	}

	// ::Config
	Object::Config::OriginPoint Object::Config::get_origin_point(const bool based_on_render_size) const
	{
		if (!based_on_render_size) return origin_point;
		return { origin_point.x * scale.x, origin_point.y * scale.y };
	}
	void Object::Config::set_origin_point(const SDL_FPoint& pos, const bool based_on_render_size)
	{
		if (based_on_render_size)
			origin_point = {
				!Utilities::Math::is_equal(scale.x, 0) ? pos.x / scale.x : 0,
				!Utilities::Math::is_equal(scale.y, 0) ? pos.y / scale.y : 0
		};
		else
			origin_point = { pos.x, pos.y };
	}
	void Object::Config::set_render_size(const SDL_FPoint& size, const SDL_FPoint& src_size)
	{
		if (src_size.x > 0) scale.x = size.x / src_size.x;
		if (src_size.y > 0) scale.y = size.y / src_size.y;
	}
	void Object::Config::set_render_size(const float& value, const SDL_FPoint& src_size)
	{
		set_render_size({ value, value }, src_size);
	}
	SDL_FRect Object::Config::get_sdl_dst_rect(const SDL_FPoint& src_size) const
	{
		using Utilities::Math::FPoint::operator*;

		const auto sdl_render_pos = get_origin_point(true).convert_pos_to_origin(render_pos, { 0, 0 });
		const auto render_size = src_size * scale;
		return Utilities::Render::merge_pos_size(sdl_render_pos, render_size);
	}
	Object::Config::Config() = default;
	Object::Config::Config(const SDL_FPoint& render_pos, const OriginPoint& origin_pos) :
		origin_point(origin_pos), render_pos(render_pos)
	{
	}
	//::Config::FlipMode
	SDL_FlipMode Object::Config::FlipMode::get_mode() const
	{
		return static_cast<SDL_FlipMode>((horizontal ? SDL_FLIP_HORIZONTAL : 0) | (vertical ? SDL_FLIP_VERTICAL : 0));
	}

	// ::
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
	Object::Config::OriginPoint Object::translate_origin_type_to_point(const Types::Render::OriginType& origin_type, const bool based_on_render_size) const
	{
		if (!based_on_render_size)
		{
			const auto src_size = Utilities::Render::get_size_from_rect(get_sdl_src_rect());
			return Config::OriginPoint::translate_origin_type_to_point(origin_type, src_size);
		}
		const auto render_size = Utilities::Render::get_size_from_rect(get_sdl_dst_rect());
		return Config::OriginPoint::translate_origin_type_to_point(origin_type, render_size);
	}
	void Object::update_origin_point(const Config::OriginPoint& custom_origin, const bool move_render_pos, const bool based_on_render_size)
	{
		const auto previous_origin = config.get_origin_point();
		config.set_origin_point(custom_origin, based_on_render_size);
		if (move_render_pos)
			config.render_pos = config.get_origin_point().convert_pos_from_origin(config.render_pos, previous_origin);
	}
	void Object::update_origin_point(const Types::Render::OriginType& origin_type, const bool move_render_pos)
	{
		update_origin_point(translate_origin_type_to_point(origin_type, false), move_render_pos, false);
	}
	Object::Object(
		Memory::Item texture,
		const Types::Render::OriginType& origin_type,
		const SDL_FPoint& render_pos) :
		src(std::move(texture))
	{
		update_origin_point(origin_type, false);
		config.render_pos = render_pos;
	}
	Object::Object(
		Memory::Item texture,
		const Config::OriginPoint& custom_origin,
		const SDL_FPoint& render_pos) :
		src(std::move(texture)), config(render_pos, custom_origin)
	{
	}

	//! ReactObject
	void ReactObject::check_event(const Events::Event::Input::MouseEvents& events) const
	{
		if (!on_event) return;
		if (!allow_hide && (!visible || config.color.a == 0)) return;
		for (const auto& event : events)
		{
			if (const SDL_FPoint pos = { .x = event.x, .y = event.y };
				Utilities::Render::is_point_in_rect(pos, get_sdl_dst_rect()))
				on_event(event);
		}
	}

	ReactObject::ReactObject(
		Memory::Item texture, 
		const Types::Render::OriginType& origin_type,
		const SDL_FPoint& render_pos,
		std::function<void(const SDL_MouseButtonEvent& event)> on_event)
		: Object(std::move(texture), origin_type, render_pos),
		on_event(std::move(on_event))
	{
	}

	ReactObject::ReactObject(
		Memory::Item texture,
		const Config::OriginPoint& custom_origin,
		const SDL_FPoint& render_pos,
		std::function<void(const SDL_MouseButtonEvent& event)> on_event)
		: Object(std::move(texture), custom_origin, render_pos),
		on_event(std::move(on_event))
	{
	}

	//! Collection
	void Collection::for_each_item(const std::function<void(const size_t& index)>& function, const bool no_duplicate)
	{
		std::unordered_set<size_t> completed_index{};

		if (render_range.empty())
			for (size_t index = 0; index < data.size(); ++index)
				function(index);
		else for (auto& [from, to] : render_range)
		{
			if (from > to) std::swap(from, to);
			const auto size = static_cast<int64_t>(data.size());
			const size_t from_ = std::clamp(from, 0LL, size);
			const size_t to_ = std::clamp(to, 0LL, size - 1);
			for (auto index = from_; index <= to_; ++index)
			{
				if (no_duplicate && completed_index.contains(index)) continue;
				function(index);
				completed_index.insert(index);
			}
		}
	}
}
