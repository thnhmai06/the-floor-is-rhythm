#include "structures/render/layers/objects/gameplay/cursor.h" // Header
#include "format/skin.h"

namespace Structures::Render::Objects::Gameplay::Cursor
{
	namespace Components
	{
		// ::Body
		Body::Body(const TextureMemory& memory) :
			Object{
				memory.find(Format::SkinFormat::Cursor::BODY),
				Types::Render::RenderOriginType::CENTRE } {
		}

		// ::Trail
		Trail::Trail(const TextureMemory& memory)
		{
			push_back(Object{
				memory.find(Format::SkinFormat::Cursor::TRAIL),
				Types::Render::RenderOriginType::CENTRE });
		}

		// ::Direction
		void Direction::update_src()
		{
			if (current_direction)
				src.item = src.memory->find(Format::SkinFormat::Cursor::DIRECTION[*current_direction]).item;
		}
		void Direction::change_current_direction_target(const Types::Game::Direction::Direction* new_target)
		{
			this->current_direction = new_target;
			update_src();
		}
		Direction::Direction(
			const TextureMemory& memory,
			const Types::Game::Direction::Direction* current_direction)
			: Object{
				  memory.find(Format::SkinFormat::Cursor::DIRECTION[current_direction
																? *current_direction
																: Types::Game::Direction::Direction::RIGHT]),
				  Types::Render::RenderOriginType::CENTRE },
			current_direction(current_direction)
		{
		}
	}

	Collection::Collection(const TextureMemory& memory, const Types::Game::Direction::Direction* current_direction)
		: Objects::Collection()
	{
		using namespace Components;

		//! Chú ý thứ tự: Body -> Trail -> Direction
		// body
		auto c_body = std::make_shared<Body>(memory);
		push_back(c_body);
		body = c_body;

		// trail
		auto c_trail = std::make_shared<Trail>(memory);
		push_back(c_trail);
		trail = c_trail;

		// direction
		auto c_direction = std::make_shared<Direction>(memory, current_direction);
		push_back(c_direction);
		direction = c_direction;
	}
}