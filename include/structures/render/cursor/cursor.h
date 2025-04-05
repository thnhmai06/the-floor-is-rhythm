#pragma once
#include "structures/render/layer.h"
#include "format/skin.h"
#include "template.h"

namespace RenderObjects::Cursor
{
	using namespace Structures::Render::RenderObjects;

	struct Cursor final : RenderObject
	{
		explicit Cursor(const TextureMemory& memory) : RenderObject {
			memory.find(SkinFormat::Cursor::BODY), Template::Render::RenderOriginType::CENTRE }
		{
		}
	};
	struct CursorTrail final : RenderObject
	{
		explicit CursorTrail(const TextureMemory& memory) : RenderObject {
			memory.find(SkinFormat::Cursor::TRAIL), Template::Render::RenderOriginType::CENTRE }
		{
		}
	};
	struct CursorDirection final : RenderObject
	{
		Template::Game::Direction::Direction current_direction = Template::Game::Direction::Direction::RIGHT;

		void update_direction(const Template::Game::Direction::Direction& direction);

		explicit CursorDirection(const TextureMemory& memory) : RenderObject{
			memory.find(SkinFormat::Cursor::DIRECTION[Template::Game::Direction::Direction::RIGHT]),
			Template::Render::RenderOriginType::CENTRE }
		{
		}
	};
}
