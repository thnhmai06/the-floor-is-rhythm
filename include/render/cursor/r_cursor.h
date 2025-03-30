#pragma once
#include "render/layer.h"
#include "game/hitobject.h"
#include "rule/config.h"
#include "utilities.h"
#include "rule/skin.h"

namespace RenderObjects::Cursor
{
	struct Cursor : RenderObject
	{
		const std::string* tail;

		void render(const Texture::TextureMemory& memory) const override;
		Cursor()
		{
			target_texture_name = &SkinFormat::Cursor::NAME;
			tail = &SkinFormat::Cursor::TAIL;
			config.origin_dst = std::make_unique<SDL_FRect>(
				Utilities::Math::centre(static_cast<float>(Config::Cursor::size), ImmutableConfig::Video::LOGICAL_WIDTH),
				Utilities::Math::centre(static_cast<float>(Config::Cursor::size), ImmutableConfig::Video::LOGICAL_HEIGHT),
				static_cast<float>(Config::Cursor::size), 
				static_cast<float>(Config::Cursor::size)
			);
		}
	};
	struct CursorDirection : RenderObject
	{
		void change_direction(Direction new_direction);
		CursorDirection()
		{
			change_direction(Direction::RIGHT); // default
			config.origin_dst = std::make_unique<SDL_FRect>(
				Utilities::Math::centre(static_cast<float>(Config::Cursor::direction_size), ImmutableConfig::Video::LOGICAL_WIDTH),
				Utilities::Math::centre(static_cast<float>(Config::Cursor::direction_size), ImmutableConfig::Video::LOGICAL_HEIGHT),
				static_cast<float>(Config::Cursor::direction_size), 
				static_cast<float>(Config::Cursor::direction_size)
			);
		}
	};
}
