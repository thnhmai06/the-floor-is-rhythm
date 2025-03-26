#pragma once
#include "render/layer.h"
#include "game/object.h"
#include "config.h"
#include "utilities.h"

namespace RenderObjects
{
	struct Cursor : RenderObject
	{
		std::string tail;
		SDL_FRect dst_rect = {
			Utilities::Math::centre(Config::Cursor::size,Immutable::Video::LOGICAL_WIDTH),
			Utilities::Math::centre(Config::Cursor::size,Immutable::Video::LOGICAL_HEIGHT),
			static_cast<float>(Config::Cursor::size), static_cast<float>(Config::Cursor::size)
		};

		void render(const TextureMemory& memory) const override;
		Cursor();
	};
	struct CursorDirection : RenderObject
	{
		SDL_FRect dst_rect = {
			Utilities::Math::centre(Config::Cursor::direction_size,Immutable::Video::LOGICAL_WIDTH),
			Utilities::Math::centre(Config::Cursor::direction_size,Immutable::Video::LOGICAL_HEIGHT),
			static_cast<float>(Config::Cursor::direction_size), static_cast<float>(Config::Cursor::direction_size)
		};

		std::string change_direction(Direction new_direction);
		CursorDirection();
	};
}