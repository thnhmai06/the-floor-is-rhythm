#pragma once
#include "render/texture.h"
#include "template.h"

struct RenderConfig
{
	struct RenderOriginPoint : SDL_FPoint
	{
		[[nodiscard]] SDL_FPoint to_sdl_pos(const SDL_FPoint& texture_pos) const; // SDL
		[[nodiscard]] SDL_FPoint from_sdl_pos(const SDL_FPoint& sdl_pos) const; // current
		[[nodiscard]] SDL_FRect to_sdl_dst(const SDL_FPoint& render_pos, const SDL_FPoint& texture_size) const;
		[[nodiscard]] SDL_FRect to_sdl_dst(const SDL_FRect& render_rect) const;
		[[nodiscard]] SDL_FRect from_sdl_dst(const SDL_FPoint& sdl_pos, const SDL_FPoint& texture_size) const;
		[[nodiscard]] SDL_FRect from_sdl_dst(const SDL_FRect& dst_rect) const;
	};
	SDL_FPoint render_pos = { 0, 0 };
	RenderOriginPoint origin_pos = { 0, 0 };
	SDL_FPoint scale = { 1.0f, 1.0f };
	uint8_t alpha = 255;

	void set_both_scale(const float& value) { scale = { .x = value, .y = value }; }
	void set_scale_fixed(const SDL_FPoint& dst_size, const SDL_FPoint& texture_size);
	void set_scale_fixed(const float& value, const SDL_FPoint& texture_size);
	[[nodiscard]] SDL_FRect to_sdl_dst(const SDL_FPoint& texture_size) const;

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
		const std::string* name;
		const TextureMemory* memory;
		SDL_FRect src_rect_in_percent = { 0, 0, 1, 1 };
		RenderConfig config;

		[[nodiscard]] SDL_FRect get_src_rect_fixed() const;
		[[nodiscard]] SDL_FRect to_sdl_dst() const;
		[[nodiscard]] SDL_FRect from_sdl_dst(const SDL_FPoint& sdl_pos) const;
		void set_scale_fixed(const SDL_FPoint& dst_size);
		virtual void render() const;

		explicit RenderObject(const std::string* name, const TextureMemory* memory, const Template::Render::RenderOriginType& origin_type);
		explicit RenderObject(const std::string* name, const TextureMemory* memory, const RenderConfig::RenderOriginPoint& custom_origin) :
			name(name), memory(memory), config(custom_origin) {
		}
		virtual ~RenderObject() = default;
	};
	using RenderObjects = std::vector<RenderObject>;
}
