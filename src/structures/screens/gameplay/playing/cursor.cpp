#include "structures/screens/gameplay/playing/cursor.h" // Header
#include "format/skin.h"

namespace Structures::Screens::Gameplay::Playing::Cursor
{
	namespace Components
	{
		// ::Body
		Body::Body(const TextureMemory& memory) :
			Object{
				memory.find(Format::Skin::Cursor::body),
				Types::Render::OriginType::CENTRE } {
		}

		// ::Trail
		Trail::Trail(const TextureMemory& memory)
		{
			data.emplace_back(memory.find(Format::Skin::Cursor::trail), Types::Render::OriginType::CENTRE);
		}

		// ::Direction
		void Direction::update_src()
		{
			if (current_direction)
				src.item = src.memory->find(Format::Skin::Cursor::direction_skin[*current_direction]).item;
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
				  memory.find(Format::Skin::Cursor::direction_skin[current_direction
																? *current_direction
																: Types::Game::Direction::Direction::RIGHT]),
				  Types::Render::OriginType::CENTRE },
			current_direction(current_direction)
		{
		}
	}

	Cursor::Cursor(const TextureMemory& memory, const Types::Game::Direction::Direction* current_direction)
		: Collection()
	{
		using namespace Components;
		data.reserve(3);

		//! Chú ý thứ tự: Body -> Trail -> Direction
		// body
		auto c_body = std::make_shared<Body>(memory);
		body = c_body;
		data.emplace_back(std::move(c_body));

		// trail
		auto c_trail = std::make_shared<Trail>(memory);
		trail = c_trail;
		data.emplace_back(std::move(c_trail));

		// direction
		auto c_direction = std::make_shared<Direction>(memory, current_direction);
		direction = c_direction;
		data.emplace_back(std::move(c_direction));
	}
}