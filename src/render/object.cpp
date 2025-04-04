#include "render/object.h" // Header
#include "logger/exceptions.h"
#include "logger/logging.h"
#include "utilities.h"

// RenderConfig::RenderOrigin
SDL_FPoint RenderConfig::RenderOriginPoint::convert_pos_from_origin(const SDL_FPoint& pos, const RenderOriginPoint& from_origin) const
{
	return { pos.x - from_origin.x + x , pos.y - from_origin.y + y };
}
SDL_FPoint RenderConfig::RenderOriginPoint::convert_pos_to_origin(const SDL_FPoint& pos, const RenderOriginPoint& to_origin) const
{
	return { pos.x - x + to_origin.x, pos.y - y + to_origin.y };
}
SDL_FRect RenderConfig::RenderOriginPoint::convert_rect_from_origin(const SDL_FRect& rect, const RenderOriginPoint& to_origin) const
{
	const auto [new_x, new_y] = convert_pos_from_origin({ .x = rect.x, .y = rect.y }, to_origin);
	return { new_x, new_y, rect.w, rect.h };
}
SDL_FRect RenderConfig::RenderOriginPoint::convert_rect_to_origin(const SDL_FRect& rect, const RenderOriginPoint& to_origin) const
{
	const auto [new_x, new_y] = convert_pos_to_origin({ .x = rect.x, .y = rect.y }, to_origin);
	return { new_x, new_y, rect.w, rect.h };
}

// RenderConfig
void RenderConfig::set_scale_fixed(const SDL_FPoint& size, const SDL_FPoint& src_size)
{
	if (src_size.x > 0) scale.x = size.x / src_size.x;
	if (src_size.y > 0) scale.y = size.y / src_size.y;
}
void RenderConfig::set_scale_fixed(const float& value, const SDL_FPoint& src_size)
{
	set_scale_fixed({ value, value }, src_size);
}
SDL_FRect RenderConfig::get_sdl_dst_rect(const SDL_FPoint& src_size) const
{
	return {
		render_pos.x - origin_pos.x * scale.x,
		render_pos.y - origin_pos.y * scale.y,
		src_size.x * scale.x,
		src_size.y * scale.y
	};
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
SDL_FPoint RenderObjects::RenderObject::get_render_sdl_pos() const
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
void RenderObjects::RenderObject::set_origin_pos(const Template::Render::RenderOriginType& origin_type)
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
void RenderObjects::RenderObject::set_origin_pos(const RenderConfig::RenderOriginPoint& custom_origin)
{
	config.origin_pos = custom_origin;
}
void RenderObjects::RenderObject::render() const
{
	const auto sdl_texture = src.item->second;
	const auto src_rect = get_sdl_src_rect();
	const auto dst_rect = get_sdl_dst_rect();
	SDL_SetTextureAlphaMod(sdl_texture, config.alpha);
	if (!SDL_RenderTexture(src.memory->renderer, sdl_texture, &src_rect, &dst_rect))
		THROW_ERROR(SDLExceptions::Texture::SDL_Texture_Render_Failed(src.get_name()));
}
RenderObjects::RenderObject::RenderObject(Texture texture, const Template::Render::RenderOriginType& origin_type) :
	src(std::move(texture))
{
	set_origin_pos(origin_type);
}
RenderObjects::RenderObject::RenderObject(Texture texture, const RenderConfig::RenderOriginPoint& custom_origin) :
	src(std::move(texture)), config(custom_origin) {
}