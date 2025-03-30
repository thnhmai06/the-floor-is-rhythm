#include "render/cursor/r_cursor.h" // Header
#include "exceptions.h"
#include "logging.h"

//! Cursor
void RenderObjects::Cursor::Cursor::render(const Texture::TextureMemory& memory) const
{
	if (memory.render(*tail, config))
		LOG_ERROR(SDLExceptions::Texture::SDL_Texture_Render_Failed(*tail));
	if (!memory.render(*target_texture_name, config))
		LOG_ERROR(SDLExceptions::Texture::SDL_Texture_Render_Failed(*target_texture_name));
}
//! CursorDirection
void RenderObjects::Cursor::CursorDirection::change_direction(const Direction new_direction)
{
	target_texture_name = &SkinFormat::Cursor::DIRECTION.find(new_direction)->second;
}