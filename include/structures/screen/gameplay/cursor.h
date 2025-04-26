#pragma once
#include "structures/render/object.h"

namespace Structures::Screen::Gameplay::Cursor
{
	namespace Render
	{
		using namespace Structures::Render::Object;

		namespace Components
		{
			struct NoteScore : Object
			{
				void set_display_score(const Types::Game::Gameplay::NoteScore& score);

				using Object::Object;
			};
		}

		struct Cursor : Collection
		{
			std::weak_ptr<Object> body;
			std::weak_ptr<Components::NoteScore> note_score;

			explicit Cursor(const Memory& memory);
		};
	}
}