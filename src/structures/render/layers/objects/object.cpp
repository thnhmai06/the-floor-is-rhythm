#include "structures/render/layers/objects/object.h" // Header
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "utilities.hpp"

namespace Structures::Render::Objects
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
		const SDL_FPoint& size, const Types::Render::RenderOriginType& origin_type)
	{
		const auto [w, h] = size;
		switch (origin_type)
		{
		case Types::Render::RenderOriginType::TOP_LEFT:
			return { 0, 0 };
		case Types::Render::RenderOriginType::BOTTOM_LEFT:
			return { 0, h };
		case Types::Render::RenderOriginType::BOTTOM_RIGHT:
			return { w, h };
		case Types::Render::RenderOriginType::TOP_RIGHT:
			return { w, 0 };
		case Types::Render::RenderOriginType::TOP_CENTRE:
			return { w / 2, 0 };
		case Types::Render::RenderOriginType::BOTTOM_CENTRE:
			return { w / 2, h };
		case Types::Render::RenderOriginType::CENTRE_LEFT:
			return { 0, h / 2 };
		case Types::Render::RenderOriginType::CENTRE_RIGHT:
			return { w, h / 2 };
		case Types::Render::RenderOriginType::CENTRE:
			return { w / 2, h / 2 };
		}
		return { 0, 0 };
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
	void Object::Config::set_scale(const SDL_FPoint& value)
	{
		const auto old_render_origin = get_origin_point(true);
		scale = value;
		const auto new_render_origin = get_origin_point(true);

		render_pos.x += old_render_origin.x - new_render_origin.x;
		render_pos.y += old_render_origin.y - new_render_origin.y;
	}
	void Object::Config::set_scale(const float& value) { set_scale({ value, value }); }
	void Object::Config::set_fixed_render_size(const SDL_FPoint& size, const SDL_FPoint& src_size)
	{
		if (src_size.x > 0) scale.x = size.x / src_size.x;
		if (src_size.y > 0) scale.y = size.y / src_size.y;
	}
	void Object::Config::set_fixed_render_size(const float& value, const SDL_FPoint& src_size)
	{
		set_fixed_render_size({ value, value }, src_size);
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
		config.set_fixed_render_size(size, Utilities::Render::get_size_from_rect(get_sdl_src_rect()));
	}
	void Object::set_render_size(const float& value)
	{
		set_render_size({ value, value });
	}
	Object::Config::OriginPoint Object::translate_origin_type_to_point(const Types::Render::RenderOriginType& origin_type, const bool based_on_render_size) const
	{
		if (!based_on_render_size)
		{
			const auto src_size = Utilities::Render::get_size_from_rect(get_sdl_src_rect());
			return Config::OriginPoint::translate_origin_type_to_point(src_size, origin_type);
		}
		const auto render_size = Utilities::Render::get_size_from_rect(get_sdl_dst_rect());
		return Config::OriginPoint::translate_origin_type_to_point(render_size, origin_type);
	}
	void Object::update_origin_point(const Config::OriginPoint& custom_origin, const bool move_render_pos, const bool based_on_render_size)
	{
		const auto previous_origin = config.get_origin_point();
		config.set_origin_point(custom_origin, based_on_render_size);
		if (move_render_pos)
			config.render_pos = config.get_origin_point().convert_pos_from_origin(config.render_pos, previous_origin);
	}
	void Object::update_origin_point(const Types::Render::RenderOriginType& origin_type, const bool move_render_pos)
	{
		update_origin_point(translate_origin_type_to_point(origin_type, false), move_render_pos, false);
	}
	void Object::render(const SDL_FPoint& offset) const
	{
		if (!visible) return;

		const auto sdl_texture = src.item->second;
		const auto src_rect = get_sdl_src_rect();
		auto dst_rect = get_sdl_dst_rect();
		dst_rect.x += offset.x; dst_rect.y += offset.y;
		const SDL_FPoint render_origin_point = config.get_origin_point(true);
		SDL_SetTextureAlphaMod(sdl_texture, config.alpha);

		if (!SDL_RenderTextureRotated(src.memory->renderer, sdl_texture, &src_rect, 
			&dst_rect, config.angle, &render_origin_point, config.flip_mode))
			THROW_ERROR(Logging::Exceptions::SDLExceptions::Texture::SDL_Texture_Render_Failed(src.get_name()));

	}
	Object::Object(
		TextureMemory::Item texture,
		const Types::Render::RenderOriginType& origin_type,
		const SDL_FPoint& render_pos) :
		src(std::move(texture))
	{
		update_origin_point(origin_type, false);
		config.render_pos = render_pos;
	}
	Object::Object(
		TextureMemory::Item texture,
		const Config::OriginPoint& custom_origin,
		const SDL_FPoint& render_pos) :
		src(std::move(texture)), config(render_pos, custom_origin)
	{
	}

	//! PolyObject
	auto PolyObject::operator+=(const CONTAINER& other)
	{
		for (const auto& object : other)
			data.push_back(object);
		return *this;
	}
	auto PolyObject::operator+=(const Object& obj)
	{
		data.push_back(obj);
		return *this;
	}
	void PolyObject::render(const SDL_FPoint& offset) const
	{
		if (!visible) return;
		for (const auto& object : data)
			object.render(offset);
	}
}