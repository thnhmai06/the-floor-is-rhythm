#pragma once
#include "structures/render/layers/objects/gameplay/mapset.h"
#include "structures/render/layers/objects/collection.h"
#include "template.h"

namespace Structures::Render::RenderObjects::Cursor
{
	namespace Components
	{
		struct RenderCursorBody final : RenderObject
		{
			explicit RenderCursorBody(const TextureMemory& memory);
		};
		struct RenderCursorTrail final : PolyRenderObject
		{
			//TODO: Thêm trail theo hướng di chuyển
			explicit RenderCursorTrail(const TextureMemory& memory);
		};
		struct RenderCursorDirection final : RenderObject
		{
		private:
			const Template::Game::Direction::Direction* current_direction;

		public:
			void update_src();
			void change_current_direction_target(const Template::Game::Direction::Direction* new_target);
			RenderCursorDirection(const TextureMemory& memory, const Template::Game::Direction::Direction* current_direction);
		};
	}

	namespace Collection
	{
		struct CursorCollection final : RenderObjectCollection
		{
		protected:
			using BASE = RenderObjectCollection;

		public:
			using BASE::render;
			std::weak_ptr<Components::RenderCursorBody> body;
			std::weak_ptr<Components::RenderCursorTrail> trail;
			std::weak_ptr<Components::RenderCursorDirection> direction;

			CursorCollection(const TextureMemory& memory, const Template::Game::Direction::Direction* current_direction);
		};
	}
}