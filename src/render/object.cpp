#include "render/object.h" // Header
#include "logger/exceptions.h"
#include "logger/logging.h"
#include "utilities.h"

// RenderConfig::RenderOrigin
SDL_FPoint RenderConfig::RenderOriginPoint::convert_pos_from_origin(const SDL_FPoint& pos, const RenderOriginPoint& from_origin) const
{
	return { pos.x - from_origin.x + x, pos.y - from_origin.y + y };
}
SDL_FPoint RenderConfig::RenderOriginPoint::convert_pos_to_origin(const SDL_FPoint& pos, const RenderOriginPoint& to_origin) const
{
	return { pos.x + to_origin.x - x, pos.y + to_origin.y - y };
}
SDL_FRect RenderConfig::RenderOriginPoint::convert_rect_from_origin(const SDL_FRect& rect, const RenderOriginPoint& to_origin) const
{
	const auto [new_x, new_h] = convert_pos_from_origin({ .x = rect.x, .y = rect.y }, to_origin);
	return { new_x, new_h, rect.w, rect.h };
}
SDL_FRect RenderConfig::RenderOriginPoint::convert_rect_to_origin(const SDL_FRect& rect, const RenderOriginPoint& to_origin) const
{
	const auto [new_x, new_h] = convert_pos_to_origin({ .x = rect.x, .y = rect.y }, to_origin);
	return { new_x, new_h, rect.w, rect.h };
}

// RenderConfig
void RenderConfig::set_scale_fixed(const SDL_FPoint& size, const SDL_FPoint& src_size)
{
	scale.x = size.x / src_size.x;
	scale.y = size.y / src_size.y;
}
void RenderConfig::set_scale_fixed(const float& value, const SDL_FPoint& src_size)
{
	set_scale_fixed({ value, value }, src_size);
}
SDL_FRect RenderConfig::get_sdl_dst_rect(const SDL_FPoint& src_size) const
{
	return origin_pos.convert_rect_to_origin(
		{ .x = render_pos.x,
			.y = render_pos.y,
			.w = src_size.x,
			.h = src_size.y});
}

//! RenderObjects
// ::RenderObject
SDL_FRect RenderObjects::RenderObject::get_sdl_src_rect() const
{
	const auto [src_w, src_h] = src.get_size();
	return { src_w * src_rect_in_percent.x, src_h * src_rect_in_percent.y, src_w * src_rect_in_percent.w, src_h * src_rect_in_percent.h };
}
SDL_FRect RenderObjects::RenderObject::get_sdl_dst_rect() const
{
	return config.get_sdl_dst_rect(Utilities::Render::get_size_from_rect(get_sdl_src_rect()));
}
SDL_FPoint RenderObjects::RenderObject::get_render_pos() const
{
	return Utilities::Render::get_pos_from_rect(get_sdl_dst_rect());
}
SDL_FPoint RenderObjects::RenderObject::get_render_size() const
{
	return Utilities::Render::get_size_from_rect(get_sdl_dst_rect());
}
void RenderObjects::RenderObject::set_scale_fixed(const SDL_FPoint& size)
{
	config.set_scale_fixed(size, Utilities::Render::get_size_from_rect(get_sdl_src_rect()));
}
void RenderObjects::RenderObject::set_scale_fixed(const float& value)
{
	config.set_scale_fixed(value, Utilities::Render::get_size_from_rect(get_sdl_src_rect()));
}
void RenderObjects::RenderObject::render() const
{
	const auto item = src.item;
	if (item == src.memory->end())
		THROW_ERROR(SDLExceptions::Texture::SDL_Texture_Render_Failed(src.get_name()));

	const auto sdl_texture = item->second;
	const auto src_rect = get_sdl_src_rect();
	const auto dst_rect = get_sdl_dst_rect();
	SDL_SetTextureAlphaMod(sdl_texture, config.alpha);
	if (!SDL_RenderTexture(src.memory->renderer, sdl_texture, &src_rect, &dst_rect))
		THROW_ERROR(SDLExceptions::Texture::SDL_Texture_Render_Failed(src.get_name()));
}
RenderObjects::RenderObject::RenderObject(const Texture& texture, const Template::Render::RenderOriginType& origin_type) : src(texture)
{
	const auto [w, h] = Utilities::Render::get_size_from_rect(get_sdl_src_rect());
	switch (origin_type)
	{
	case Template::Render::RenderOriginType::TOP_LEFT:
		config.origin_pos = { 0, 0 };
		break;
	case Template::Render::RenderOriginType::CENTRE:
		config.origin_pos = { w / 2, h / 2 };
		break;
	case Template::Render::RenderOriginType::BOTTOM_LEFT:
		config.origin_pos = { 0, h };
		break;
	case Template::Render::RenderOriginType::BOTTOM_RIGHT:
		config.origin_pos = { w, h };
		break;
	case Template::Render::RenderOriginType::TOP_RIGHT:
		config.origin_pos = { w, 0 };
		break;
	case Template::Render::RenderOriginType::TOP_CENTRE:
		config.origin_pos = { w / 2, 0 };
		break;
	case Template::Render::RenderOriginType::BOTTOM_CENTRE:
		config.origin_pos = { w / 2, h };
		break;
	case Template::Render::RenderOriginType::CENTRE_LEFT:
		config.origin_pos = { 0, h / 2 };
		break;
	case Template::Render::RenderOriginType::CENTRE_RIGHT:
		config.origin_pos = { w, h / 2 };
		break;
	}
}
RenderObjects::RenderObject::RenderObject(const Texture& texture, const RenderConfig::RenderOriginPoint& custom_origin) : src(texture), config(custom_origin) {}