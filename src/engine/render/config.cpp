#include "engine/render/config.h" // Header
#include <SDL3/SDL_surface.h>

namespace Engine::Render::Config
{
	//! OriginPoint
	SDL_FPoint OriginPoint::convert_pos_from_origin(const SDL_FPoint& pos, const OriginPoint& from_origin) const
	{
		return { pos.x - from_origin.x + x , pos.y - from_origin.y + y };
	}
	SDL_FPoint OriginPoint::convert_pos_to_origin(const SDL_FPoint& pos, const OriginPoint& to_origin) const
	{
		return { pos.x - x + to_origin.x, pos.y - y + to_origin.y };
	}
	SDL_FRect OriginPoint::convert_rect_from_origin(const SDL_FRect& rect, const OriginPoint& to_origin) const
	{
		const auto [new_x, new_y] = convert_pos_from_origin({ .x = rect.x, .y = rect.y }, to_origin);
		return { new_x, new_y, rect.w, rect.h };
	}
	SDL_FRect OriginPoint::convert_rect_to_origin(const SDL_FRect& rect, const OriginPoint& to_origin) const
	{
		const auto [new_x, new_y] = convert_pos_to_origin({ .x = rect.x, .y = rect.y }, to_origin);
		return { new_x, new_y, rect.w, rect.h };
	}
	OriginPoint OriginPoint::translate_origin_type_to_point(
		const OriginType& origin_type, const SDL_FPoint& size)
	{
		const auto& [w, h] = size;
		switch (origin_type)
		{
		case OriginType::TopLeft:
		case OriginType::Custom:
			return { 0, 0 };
		case OriginType::BottomLeft:
			return { 0, h };
		case OriginType::BottomRight:
			return { w, h };
		case OriginType::TopRight:
			return { w, 0 };
		case OriginType::TopCentre:
			return { w / 2, 0 };
		case OriginType::BottomCentre:
			return { w / 2, h };
		case OriginType::CentreLeft:
			return { 0, h / 2 };
		case OriginType::CentreRight:
			return { w, h / 2 };
		case OriginType::Centre:
			return { w / 2, h / 2 };
		}
		return { 0, 0 };
	}
	OriginPoint::OriginPoint(
		const OriginType& origin_type, const SDL_FPoint& size)
		: OriginPoint(translate_origin_type_to_point(origin_type, size))
	{
	}

	//! RenderConfig
	RenderConfig::RenderConfig(const SDL_FPoint& render_pos, const double& alpha)
		: render_pos(render_pos)
	{
		this->alpha.percent = alpha;
	}
	bool RenderConfig::is_visible() const
	{
		return visible && alpha.get_percent() > 0;
	}

	//! ObjectConfig
	//::FlipMode
	SDL_FlipMode ObjectConfig::FlipMode::get_mode() const
	{
		return static_cast<SDL_FlipMode>((horizontal ? SDL_FLIP_HORIZONTAL : 0) | (vertical ? SDL_FLIP_VERTICAL : 0));
	}
	// ::
	OriginPoint ObjectConfig::get_origin_point(const bool based_on_render_size) const
	{
		if (!based_on_render_size) return origin_point;
		return { origin_point.x * scale.x, origin_point.y * scale.y };
	}
	void ObjectConfig::set_origin_point(const SDL_FPoint& pos, const bool based_on_render_size)
	{
		if (based_on_render_size)
			origin_point = {
				!Utilities::Math::is_equal(scale.x, 0) ? pos.x / scale.x : 0,
				!Utilities::Math::is_equal(scale.y, 0) ? pos.y / scale.y : 0
		};
		else
			origin_point = { pos.x, pos.y };
	}
	void ObjectConfig::set_render_size(const SDL_FPoint& size, const SDL_FPoint& src_size)
	{
		if (src_size.x > 0) scale.x = size.x / src_size.x;
		if (src_size.y > 0) scale.y = size.y / src_size.y;
	}
	void ObjectConfig::set_render_size(const float& value, const SDL_FPoint& src_size)
	{
		set_render_size({ value, value }, src_size);
	}
	SDL_FRect ObjectConfig::get_sdl_dst_rect(const SDL_FPoint& src_size) const
	{
		using Utilities::Math::FPoint::operator*;

		const auto sdl_render_pos = get_origin_point(true).convert_pos_to_origin(render_pos, { 0, 0 });
		const auto render_size = src_size * scale;
		return Utilities::Render::merge_pos_size(sdl_render_pos, render_size);
	}
	ObjectConfig::ObjectConfig(const SDL_FPoint& render_pos, const OriginPoint& origin_pos) :
		RenderConfig(render_pos), origin_point(origin_pos)
	{
	}
}
