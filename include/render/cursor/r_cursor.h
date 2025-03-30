#pragma once
#include "render/layer.h"
#include "rule/skin.h"
#include "template.h"

namespace RenderObjects::Cursor
{
	struct Cursor final : RenderObject
	{
		explicit Cursor(const TextureMemory* skin) : RenderObject(
			&SkinFormat::Cursor::BODY, skin, Template::Render::RenderOriginType::CENTRE)
		{
		}
	};
	struct CursorTail final : RenderObject
	{
		explicit CursorTail(const TextureMemory* skin) : RenderObject(
			&SkinFormat::Cursor::TAIL, skin, Template::Render::RenderOriginType::CENTRE)
		{
		}
	};
	struct CursorDirection final : RenderObject
	{
		void change_direction(const Template::Game::Direction::Direction& direction);

		explicit CursorDirection(const TextureMemory* skin) : RenderObject(
			&SkinFormat::Cursor::DIRECTION[Template::Game::Direction::Direction::Direction::RIGHT], skin, Template::Render::RenderOriginType::CENTRE)
		{
		}
	};
}
