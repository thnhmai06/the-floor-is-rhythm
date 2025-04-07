#include "structures/render/cursor/cursor.h" // Header
#include "format/skin.h"

//! Structures::Render::PolyRenderObject::Cursor
//! ::Components
using namespace Structures::Render::RenderObjects::Cursor::Components;
// ::RenderCursorBody
RenderCursorBody::RenderCursorBody(const TextureMemory& memory) :
	RenderObject{
		memory.find(SkinFormat::Cursor::BODY),
		Template::Render::RenderOriginType::CENTRE } {
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
		src.item = src.memory->find(SkinFormat::Cursor::DIRECTION[*current_direction]).item;
}
void RenderCursorDirection::change_current_direction_target(const Template::Game::Direction::Direction* new_target)
{
	this->current_direction = new_target;
	update_src();
}
RenderCursorDirection::RenderCursorDirection(
	const TextureMemory& memory,
	const Template::Game::Direction::Direction* current_direction)
	: RenderObject{
		  memory.find(SkinFormat::Cursor::DIRECTION[current_direction
														? *current_direction
														: Template::Game::Direction::Direction::RIGHT]),
		  Template::Render::RenderOriginType::CENTRE },
	current_direction(current_direction)
{
}

//! ::Collection
using namespace Structures::Render::RenderObjects::Cursor::Collection;
CursorCollection::CursorCollection(const TextureMemory& memory, const Template::Game::Direction::Direction* current_direction)
: RenderObjectCollection()
{
	auto c_body = std::make_shared<RenderCursorBody>(memory);
	push_back(c_body);
	body = c_body;

	auto c_trail = std::make_shared<RenderCursorTrail>(memory);
	push_back(c_trail);
	trail = c_trail;

	auto c_direction = std::make_shared<RenderCursorDirection>(memory, current_direction);
	push_back(c_direction);
	direction = c_direction;
}