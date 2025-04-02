#pragma once
#include "render/layer.h"
#include "rule/skin.h"
#include "template.h"

namespace RenderObjects::Cursor
{
	struct Cursor final : RenderObject
	{
		explicit Cursor(const TextureMemory* memory) : RenderObject {
			memory->find(SkinFormat::Cursor::BODY), Template::Render::RenderOriginType::CENTRE }
		{
		}
	};
	struct CursorTail final : RenderObject
	{
		explicit CursorTail(const TextureMemory* memory) : RenderObject {
			memory->find(SkinFormat::Cursor::TAIL), Template::Render::RenderOriginType::CENTRE }
		{
		}
	};
	struct CursorDirection final : RenderObject
	{
		Template::Game::Direction::Direction current_direction = Template::Game::Direction::Direction::RIGHT;

		void update_direction(const Template::Game::Direction::Direction& direction);

		explicit CursorDirection(const TextureMemory* memory) : RenderObject{
			memory->find(SkinFormat::Cursor::DIRECTION[current_direction]), Template::Render::RenderOriginType::CENTRE }
		{
		}
	};
}
