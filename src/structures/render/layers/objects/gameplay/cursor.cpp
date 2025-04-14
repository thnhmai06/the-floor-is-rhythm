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
			data.emplace_back(memory.find(Format::SkinFormat::Cursor::TRAIL), Types::Render::RenderOriginType::CENTRE);
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
		ObjectUnique c_body = std::make_unique<Body>(memory);
		data.emplace_back(std::move(c_body));
		body = dynamic_cast<Body*>(std::get_if<ObjectUnique>(&data.back())->get());

		// trail
		auto c_trail = std::make_unique<Trail>(memory);
		data.emplace_back(std::move(c_trail));
		trail = dynamic_cast<Trail*>(std::get_if<PolyObjectUnique>(&data.back())->get());

		// direction
		auto c_direction = std::make_unique<Direction>(memory, current_direction);
		data.emplace_back(std::move(c_direction));
		direction = dynamic_cast<Direction*>(std::get_if<ObjectUnique>(&data.back())->get());
	}
}