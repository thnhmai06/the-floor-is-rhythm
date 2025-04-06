#include "structures/render/cursor/cursor.h" // Header
#include "format/skin.h"

//! Structures::Render::RenderObjects::Cursor
using namespace Structures::Render::RenderObjects::Cursor;
// ::RenderCursorBody
RenderCursorBody::RenderCursorBody(const TextureMemory& memory)
{
	push_back(RenderObject{
		memory.find(SkinFormat::Cursor::BODY),
		Template::Render::RenderOriginType::CENTRE });
}

// ::RenderCursorTrail
RenderCursorTrail::RenderCursorTrail(const TextureMemory& memory)
{
	push_back(RenderObject{
		memory.find(SkinFormat::Cursor::TRAIL),
		Template::Render::RenderOriginType::CENTRE });
}

// ::RenderCursorDirection
void RenderCursorDirection::update_src()
{
	if (current_direction)
		front().src.item = front().src.memory->find(SkinFormat::Cursor::DIRECTION[*current_direction]).item;
}
void RenderCursorDirection::change_current_direction_target(const Template::Game::Direction::Direction* new_target)
{
	this->current_direction = new_target;
	update_src();
}
RenderCursorDirection::RenderCursorDirection(const TextureMemory& memory,
                                             const Template::Game::Direction::Direction* current_direction)
	: current_direction(current_direction)
{
	push_back(RenderObject{
		memory.find(SkinFormat::Cursor::DIRECTION[this->current_direction
													  ? *this->current_direction
													  : Template::Game::Direction::Direction::RIGHT]),
		Template::Render::RenderOriginType::CENTRE });
}