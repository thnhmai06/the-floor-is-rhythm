#pragma once
#include "render/texture.h"
#include "template.h"

struct RenderConfig
{
	struct RenderOriginPoint : SDL_FPoint
	{
		[[nodiscard]] SDL_FPoint convert_pos_from_origin(const SDL_FPoint& pos, const RenderOriginPoint& from_origin = { 0, 0 }) const;
		[[nodiscard]] SDL_FPoint convert_pos_to_origin(const SDL_FPoint& pos, const RenderOriginPoint& to_origin = { 0, 0 }) const;
		[[nodiscard]] SDL_FRect convert_rect_from_origin(const SDL_FRect& rect, const RenderOriginPoint& to_origin = { 0, 0 }) const;
		[[nodiscard]] SDL_FRect convert_rect_to_origin(const SDL_FRect& rect, const RenderOriginPoint& to_origin = { 0, 0 }) const;
	};
	SDL_FPoint render_pos = { 0, 0 }; // chính là dst_rect với size nguyên gốc (muốn đổi size hãy ra ngoài RenderObject tìm set_scale_fixed())
	RenderOriginPoint origin_pos = { 0, 0 };
	SDL_FPoint scale = { 1.0f, 1.0f };
	uint8_t alpha = 255;

	void set_both_scale(const float& value) { scale = { .x = value, .y = value }; }
	void set_scale_fixed(const SDL_FPoint& size, const SDL_FPoint& src_size);
	void set_scale_fixed(const float& value, const SDL_FPoint& src_size);
	[[nodiscard]] SDL_FRect get_sdl_dst_rect(const SDL_FPoint& src_size) const; // dst_rect trong params render của sdl

	RenderConfig() = default;
	explicit RenderConfig(const SDL_FPoint& render_pos,
		const RenderOriginPoint& origin = { 0.0f, 0.0f }) :
		render_pos(render_pos), origin_pos(origin)
	{
	}
};

namespace RenderObjects
{
	struct RenderObject
	{
		// Không còn cho tự render toàn màn hình nữa (dstrect = nullptr);
		// nếu muốn thì set config.render_pos về {0, 0}, set config.origin_pos về góc trái, rồi chỉnh set_scale_fixed là logical size của màn hình

		Texture src;
		SDL_FRect src_rect_in_percent = { 0, 0, 1, 1 }; // cũng là sdl src_rect, nhưng ở % so với gốc
		RenderConfig config;

	protected:
		[[nodiscard]] SDL_FRect get_sdl_src_rect() const;
		[[nodiscard]] SDL_FRect get_sdl_dst_rect() const;

	public:
		[[nodiscard]] SDL_FPoint get_render_pos() const;
		[[nodiscard]] SDL_FPoint get_render_size() const;
		void set_scale_fixed(const SDL_FPoint& size); // chính là đang set dst_rect size (muốn set pos vào trong RenderConfig tìm render_pos)
		void set_scale_fixed(const float& value);
		void render() const;

		explicit RenderObject(Texture texture, const Template::Render::RenderOriginType& origin_type);
		explicit RenderObject(Texture texture, const RenderConfig::RenderOriginPoint& custom_origin);
		~RenderObject() = default;
	};
	using RenderObjects = std::vector<RenderObject>;
}
