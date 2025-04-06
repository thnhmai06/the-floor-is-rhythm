#pragma once
#include "structures/render/playground/hitobject.h"
#include "template.h"

namespace Structures::Render::RenderObjects::Cursor
{
	struct RenderCursorBody final : RenderObjects
	{
		explicit RenderCursorBody(const TextureMemory& memory);
	};
	struct RenderCursorTrail final : RenderObjects
	{
		//TODO: Thêm trail theo hướng di chuyển
		explicit RenderCursorTrail(const TextureMemory& memory);
	};
	struct RenderCursorDirection final : RenderObjects
	{
	private:
		const Template::Game::Direction::Direction* current_direction;

	public:
		void update_src();
		void change_current_direction_target(const Template::Game::Direction::Direction* new_target);
		RenderCursorDirection(const TextureMemory& memory, const Template::Game::Direction::Direction* current_direction);
	};
}
