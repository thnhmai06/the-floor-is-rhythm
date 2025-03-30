#include "render/cursor/r_cursor.h" // Header

void RenderObjects::Cursor::CursorDirection::change_direction(const Template::Game::Direction::Direction& direction)
{
	name = &SkinFormat::Cursor::DIRECTION[direction];
}