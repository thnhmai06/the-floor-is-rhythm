#include "render/object.h" // Header
#include "exceptions.h"
#include "logging.h"

// ::RenderConfig::RenderOrigin
SDL_FPoint RenderConfig::RenderOriginPoint::to_sdl_pos(const SDL_FPoint& texture_pos) const
{
	return { texture_pos.x + x, texture_pos.y + y };
}
SDL_FPoint RenderConfig::RenderOriginPoint::from_sdl_pos(const SDL_FPoint& sdl_pos) const
{
	return { sdl_pos.x - x, sdl_pos.y - y };
}
SDL_FRect RenderConfig::RenderOriginPoint::to_sdl_dst(const SDL_FPoint& render_pos, const SDL_FPoint& texture_size) const
{
	const auto [x, y] = to_sdl_pos(render_pos);
	return { x, y, texture_size.x, texture_size.y };
}
SDL_FRect RenderConfig::RenderOriginPoint::to_sdl_dst(const SDL_FRect& render_rect) const
{
	return to_sdl_dst({ .x = render_rect.x, .y = render_rect.y }, { .x = render_rect.w, .y = render_rect.h });
}
SDL_FRect RenderConfig::RenderOriginPoint::from_sdl_dst(const SDL_FPoint& sdl_pos, const SDL_FPoint& texture_size) const
{
	const auto [x, y] = from_sdl_pos(sdl_pos);
	return { x, y, texture_size.x, texture_size.y };
}
SDL_FRect RenderConfig::RenderOriginPoint::from_sdl_dst(const SDL_FRect& dst_rect) const
{
	return from_sdl_dst({ .x = dst_rect.x, .y = dst_rect.y }, { .x = dst_rect.w, .y = dst_rect.h });
}

// ::RenderConfig
void RenderConfig::set_scale_fixed(const SDL_FPoint& dst_size, const SDL_FPoint& texture_size)
{
	scale.x = dst_size.x / texture_size.x;
	scale.y = dst_size.y / texture_size.y;
}
void RenderConfig::set_scale_fixed(const float& value, const SDL_FPoint& texture_size)
{
	set_scale_fixed({ value, value }, texture_size);
}
SDL_FRect RenderConfig::to_sdl_dst(const SDL_FPoint& texture_size) const
{
	return origin_pos.to_sdl_dst(render_pos, { .x = texture_size.x * scale.x, .y = texture_size.y * scale.y });
}

//! RenderObjects
// ::RenderObject
void RenderObjects::RenderObject::set_scale_fixed(const SDL_FPoint& dst_size)
{
	config.set_scale_fixed(dst_size, memory->get_texture_size(*name));
}
SDL_FRect RenderObjects::RenderObject::to_sdl_dst() const
{
	return config.to_sdl_dst(memory->get_texture_size(*name));
}
SDL_FRect RenderObjects::RenderObject::from_sdl_dst(const SDL_FPoint& sdl_pos) const
{
	return config.origin_pos.from_sdl_dst(sdl_pos, memory->get_texture_size(*name));
}
SDL_FRect RenderObjects::RenderObject::get_src_rect_fixed() const
{
	const auto [src_x, src_y] = memory->get_texture_size(*name);
	return { src_x * src_rect_in_percent.x, src_y * src_rect_in_percent.y, src_x * src_rect_in_percent.w, src_y * src_rect_in_percent.h };
}
void RenderObjects::RenderObject::render() const
{
	const auto item = memory->find(*name);
	if (item == memory->end())
		THROW_ERROR(SDLExceptions::Texture::SDL_Texture_Render_Failed(*name));

	const auto texture = item->second;
	const auto src_rect = get_src_rect_fixed();
	const auto dst_rect = to_sdl_dst();
	SDL_SetTextureAlphaMod(texture, config.alpha);
	if (!SDL_RenderTexture(memory->renderer, texture, &src_rect, &dst_rect))
		THROW_ERROR(SDLExceptions::Texture::SDL_Texture_Render_Failed(*name));
}

RenderObjects::RenderObject::RenderObject(const std::string* name,
	const TextureMemory* memory, const Template::Render::RenderOriginType& origin_type) :
	name(name), memory(memory)
{
	const auto [w, h] = memory->get_texture_size(*name);
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