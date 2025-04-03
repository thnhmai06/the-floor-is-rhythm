#include "render/cursor/cursor.h" // Header

void RenderObjects::Cursor::CursorDirection::update_direction(const Template::Game::Direction::Direction& direction)
{
	src = Texture(SkinFormat::Cursor::DIRECTION[direction], src.memory);
}