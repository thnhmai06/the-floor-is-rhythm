#include "render/playground/r_cursor.h" // Header
#include "exceptions.h"
#include "rule/skin.h"

//! Cursor
void RenderObjects::Cursor::render(const TextureMemory& memory) const
{
	if (memory.render(tail, config))
		LOG_ERROR(SDL_Exceptions::Texture::SDL_RenderTexture_Failed(tail));
	if (!memory.render(name, config))
		LOG_ERROR(SDL_Exceptions::Texture::SDL_RenderTexture_Failed(name));
}
RenderObjects::Cursor::Cursor()
{
	// file used
	name = Skin_Filename::Cursor::NAME;
	tail = Skin_Filename::Cursor::TAIL;
	// config
	config.dst_rect = &dst_rect;
}

//! CursorDirection
std::string RenderObjects::CursorDirection::change_direction(const Direction new_direction)
{
	return name = Skin_Filename::Cursor::DIRECTION.find(new_direction)->second;
}
RenderObjects::CursorDirection::CursorDirection()
{
	// file used
	change_direction(Direction::RIGHT); // default
	// config
	config.dst_rect = &dst_rect;
}