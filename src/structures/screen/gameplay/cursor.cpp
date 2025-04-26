#include "structures/screen/gameplay/cursor.h" // Header
#include "format/skin.h"
#include "config.h"

namespace Structures::Screen::Gameplay::Cursor
{
	namespace Render
	{
		namespace Components
		{
			void NoteScore::set_display_score(const Types::Game::Gameplay::NoteScore& score)
			{
				if (score == Types::Game::Gameplay::NoteScore::Skip) return;
				src.change_target(Format::Skin::Image::HitObject::hit.at(score));
			}
		}

		Cursor::Cursor(const Memory& memory)
			: Collection()
		{
			using namespace Components;
			constexpr SDL_FPoint render_pos = { Config::GameConfig::Render::Cursor::POS_X, Config::GameConfig::Render::Cursor::POS_Y };
			data.reserve(2);

			//! Body
			auto c_body = std::make_shared<Object>(
				memory.find(Format::Skin::Image::Cursor::body),
				Config::GameConfig::Render::Cursor::ORIGIN, render_pos);
			c_body->set_render_size(::Config::GameConfig::Render::Cursor::SIZE);
			body = c_body;
			data.emplace_back(std::move(c_body));

			//! NoteScore
			auto c_note_score = std::make_shared<NoteScore>(
				memory.find(
					Format::Skin::Image::HitObject::hit.at(Types::Game::Gameplay::NoteScore::Perfect)),
				Config::GameConfig::Render::Cursor::ORIGIN, render_pos);
			c_note_score->set_render_size(::Config::GameConfig::Render::Cursor::SIZE);
			note_score = c_note_score;
			data.emplace_back(std::move(c_note_score));
		}
	}
}