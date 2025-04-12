#pragma once
#include "structures/render/layers/objects/collection.h"
#include "structures/types.h"

namespace Structures::Render::Objects::Gameplay::Cursor
{
	namespace Components
	{
		struct Body final : Object
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

	struct Collection final : Objects::Collection
	{
	protected:
		using BASE = Collection;

	public:
		using BASE::render;
		std::weak_ptr<Components::Body> body;
		std::weak_ptr<Components::Trail> trail;
		std::weak_ptr<Components::Direction> direction;

		Collection(const TextureMemory& memory, const Types::Game::Direction::Direction* current_direction);
	};
}