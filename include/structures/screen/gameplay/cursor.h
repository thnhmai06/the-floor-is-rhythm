#pragma once
#include "structures/events/action/action.h"
#include "structures/events/event/event.h"
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
				int64_t end_time;

				static void add_animation(const std::weak_ptr<NoteScore>& object, Events::Action::Buffer& action_buffer, const int64_t& time_earned);

				explicit NoteScore(const Memory& skin,
					const Types::Game::Gameplay::NoteScore& score);
			};
			
		}

		struct Cursor : Collection
		{
		protected:
			std::shared_ptr<Collection> note_scores;

		public:
			const Memory* skin;
			std::shared_ptr<Object> body;
			std::list<std::shared_ptr<Components::NoteScore>> note_score_list;

			void check_and_add_score_shown(const Events::Event::Buffer& event_buffer, Events::Action::Buffer& action_buffer, const int64_t& current_time);

			explicit Cursor(const Memory& skin);
		};
	}
}