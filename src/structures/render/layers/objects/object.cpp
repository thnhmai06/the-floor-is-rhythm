#include "structures/render/layers/objects/object.h" // Header
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "utilities.h"

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
	Object::Config::OriginPoint Object::Config::OriginPoint::get_origin_point_from_type(const TextureMemory::Item& src, const Types::Render::RenderOriginType& origin_type)
	{
		const auto [w, h] = src.get_size();
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
	void Object::Config::set_both_scale(const float& value)
	{
		scale = { .x = value, .y = value };
	}
	Object::Config::Config() = default;
	Object::Config::Config(const SDL_FPoint& render_pos, const OriginPoint& origin) :
		render_pos(render_pos), origin_point(origin)
	{
	}
	void Object::Config::set_scale_fixed(const SDL_FPoint& size, const SDL_FPoint& src_size)
	{
		if (src_size.x > 0) scale.x = size.x / src_size.x;
		if (src_size.y > 0) scale.y = size.y / src_size.y;
	}
	void Object::Config::set_scale_fixed(const float& value, const SDL_FPoint& src_size)
	{
		set_scale_fixed({ value, value }, src_size);
	}
	SDL_FRect Object::Config::get_sdl_dst_rect(const SDL_FPoint& src_size) const
	{
		return {
			render_pos.x - origin_point.x * scale.x,
			render_pos.y - origin_point.y * scale.y,
			src_size.x * scale.x,
			src_size.y * scale.y
		};
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
	Object::Config::OriginPoint Object::get_origin_point_from_type(const Types::Render::RenderOriginType& origin_type) const
	{
		return Config::OriginPoint::get_origin_point_from_type(src, origin_type);
	}
	SDL_FPoint Object::get_sdl_render_pos() const
	{
		return Utilities::Render::get_pos_from_rect(get_sdl_dst_rect());
	}
	SDL_FPoint Object::get_render_size() const
	{
		return Utilities::Render::get_size_from_rect(get_sdl_dst_rect());
	}
	void Object::set_render_size(const SDL_FPoint& size)
	{
		config.set_scale_fixed(size, Utilities::Render::get_size_from_rect(get_sdl_src_rect()));
	}
	void Object::set_render_size(const float& value)
	{
		config.set_scale_fixed(value, Utilities::Render::get_size_from_rect(get_sdl_src_rect()));
	}
	void Object::set_render_size_based_on_src(const SDL_FPoint& src_percent)
	{
		const auto [src_w, src_h] = src.get_size();
		set_render_size({ src_w * src_percent.x, src_h * src_percent.y });
	}
	void Object::set_origin_point(const Types::Render::RenderOriginType& origin_type)
	{
		config.origin_point = get_origin_point_from_type(origin_type);
	}
	void Object::set_origin_point(const Config::OriginPoint& custom_origin)
	{
		config.origin_point = custom_origin;
	}
	void Object::render(const SDL_FPoint& offset) const
	{
		if (!visible) return;

		const auto sdl_texture = src.item->second;
		const auto src_rect = get_sdl_src_rect();
		auto dst_rect = get_sdl_dst_rect();
		dst_rect.x += offset.x;
		dst_rect.y += offset.y;
		SDL_SetTextureAlphaMod(sdl_texture, config.alpha);
		if (!SDL_RenderTexture(src.memory->renderer, sdl_texture, &src_rect, &dst_rect))
			THROW_ERROR(Logging::Exceptions::SDLExceptions::Texture::SDL_Texture_Render_Failed(src.get_name()));
	}
	Object::Object(
		TextureMemory::Item texture,
		const Types::Render::RenderOriginType& origin_type,
		const SDL_FPoint& render_pos) :
		src(std::move(texture))
	{
		set_origin_point(origin_type);
		config.render_pos = render_pos;
	}
	Object::Object(
		TextureMemory::Item texture,
		const Config::OriginPoint& custom_origin,
		const SDL_FPoint& render_pos) :
		src(std::move(texture)), config(custom_origin)
	{
		config.render_pos = render_pos;
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