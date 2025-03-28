#include "render/cursor/r_cursor.h" // Header
#include "exceptions.h"
#include "logging.h"

//! Cursor
void RenderObjects::Cursor::Cursor::render(const Texture::TextureMemory& memory) const
{
	if (memory.render(*tail, config))
		LOG_ERROR(SDL_Exceptions::Texture::SDL_RenderTexture_Failed(*tail));
	if (!memory.render(*name, config))
		LOG_ERROR(SDL_Exceptions::Texture::SDL_RenderTexture_Failed(*name));
}
//! CursorDirection
void RenderObjects::Cursor::CursorDirection::change_direction(const Direction new_direction)
{
	name = &SkinFormat::Cursor::DIRECTION.find(new_direction)->second;
}