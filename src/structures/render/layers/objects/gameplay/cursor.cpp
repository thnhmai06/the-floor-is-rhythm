#include "structures/render/layers/objects/gameplay/cursor.h" // Header
#include "format/skin.h"

using namespace Structures::Render::RenderObjects::Gameplay::Cursor;
//! Structures::Render::PolyRenderObject::Cursor
//! ::Components
using namespace Components;
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
using namespace Collection;
CursorCollection::CursorCollection(const TextureMemory& memory, const Template::Game::Direction::Direction* current_direction)
	: RenderObjectCollection()
{
	//! Chú ý thứ tự: Body -> Trail -> Direction

	// body
	auto c_body = std::make_shared<RenderCursorBody>(memory);
	push_back(c_body);
	body = c_body;

	// trail
	auto c_trail = std::make_shared<RenderCursorTrail>(memory);
	push_back(c_trail);
	trail = c_trail;

	// direction
	auto c_direction = std::make_shared<RenderCursorDirection>(memory, current_direction);
	push_back(c_direction);
	direction = c_direction;
}