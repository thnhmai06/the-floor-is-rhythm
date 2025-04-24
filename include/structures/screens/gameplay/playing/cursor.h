#pragma once
#include "structures/render/collection.h"
#include "structures/types.h"

namespace Structures::Screens::Gameplay::Playing::Cursor
{
	using namespace Structures::Render::Objects;

	namespace Components
	{
		struct Body : Object
		{
			explicit Body(const TextureMemory& memory);
		};
		struct Trail final : PolyObject
		{
			//TODO: Thêm trail theo hướng di chuyển
			explicit Trail(const TextureMemory& memory);
		};
		struct Direction final : Object
		{
		private:
			const Types::Game::Direction::Direction* current_direction;

		public:
			void update_src();
			void change_current_direction_target(const Types::Game::Direction::Direction* new_target);
			Direction(const TextureMemory& memory, const Types::Game::Direction::Direction* current_direction);
		};
	}

	struct Cursor : Collection
	{
		std::shared_ptr<Components::Body> body;
		std::shared_ptr<Components::Trail> trail;
		std::shared_ptr<Components::Direction> direction;

		Cursor(const TextureMemory& memory, const Types::Game::Direction::Direction* current_direction);
	};
}