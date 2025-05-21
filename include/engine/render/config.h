#pragma once
#include <SDL3/SDL_surface.h>
#include "engine/structures.h"

namespace Engine::Render::Config
{
	struct OriginPoint : SDL_FPoint
	{
		[[nodiscard]] static OriginPoint translate_origin_type_to_point(const OriginType& origin_type, const SDL_FPoint& size);
		[[nodiscard]] SDL_FPoint convert_pos_from_origin(const SDL_FPoint& pos, const OriginPoint& from_origin = { 0, 0 }) const;
		[[nodiscard]] SDL_FPoint convert_pos_to_origin(const SDL_FPoint& pos, const OriginPoint& to_origin = { 0, 0 }) const;
		[[nodiscard]] SDL_FRect convert_rect_from_origin(const SDL_FRect& rect, const OriginPoint& to_origin = { 0, 0 }) const;
		[[nodiscard]] SDL_FRect convert_rect_to_origin(const SDL_FRect& rect, const OriginPoint& to_origin = { 0, 0 }) const;

		OriginPoint() = default;
		OriginPoint(const float x, const float y) : SDL_FPoint{ .x = x, .y = y } {}
		OriginPoint(const SDL_FPoint& point) : SDL_FPoint(point) {}
		explicit OriginPoint(const OriginType& origin_type, const SDL_FPoint& size);
	};

	struct RenderConfig
	{
		SDL_FPoint render_pos;
		PercentValue<uint8_t> alpha{ 0, SDL_MAX_ALPHA, 1 };
		bool visible = true;

		[[nodiscard]] bool is_visible() const;

		friend RenderConfig operator*(const RenderConfig& lft, const RenderConfig& rgt)
		{
			using Utilities::Math::FPoint::operator+=;

			auto res = lft;
			res.render_pos += rgt.render_pos;
			res.alpha.percent *= rgt.alpha.get_percent();
			return res;
		}
		friend RenderConfig& operator*=(RenderConfig& lft, const RenderConfig & rgt)
		{
			return lft = lft * rgt;
		}
		friend RenderConfig operator/(const RenderConfig& lft, const RenderConfig& rgt)
		{
			using Utilities::Math::FPoint::operator-=;

			auto res = lft;
			res.render_pos -= rgt.render_pos;
			res.alpha.percent /= rgt.alpha.get_percent();
			return res;
		}
		friend RenderConfig& operator/=(RenderConfig& lft, const RenderConfig& rgt)
		{
			return lft = lft / rgt;
		}

		RenderConfig() = default;
		explicit RenderConfig(const SDL_FPoint& render_pos, const double& alpha = 1);
		virtual ~RenderConfig() = default;
	};
	struct ObjectConfig : RenderConfig
	{
	protected:
		OriginPoint origin_point = { 0, 0 }; // theo src

	public:
		SDL_FPoint scale = { 1.0f, 1.0f };
		double angle = 0.0f;
		Color color = { .r = 255, .g = 255, .b = 255 };
		struct FlipMode
		{
			bool horizontal = false; // ngang
			bool vertical = false; // dọc

			[[nodiscard]] SDL_FlipMode get_mode() const;
		} flip_mode;
		SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND;

		[[nodiscard]] OriginPoint get_origin_point(bool based_on_render_size = false) const;
		[[nodiscard]] SDL_FRect get_sdl_dst_rect(const SDL_FPoint& src_size) const;
		void set_origin_point(const SDL_FPoint& pos, bool based_on_render_size = false);
		void set_render_size(const SDL_FPoint& size, const SDL_FPoint& src_size);
		void set_render_size(const float& value, const SDL_FPoint& src_size);

		ObjectConfig() = default;
		explicit ObjectConfig(const SDL_FPoint& render_pos, const OriginPoint& origin_pos);
	};
}