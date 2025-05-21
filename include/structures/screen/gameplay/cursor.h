#pragma once
#include "core/type.h"
#include "engine/events/action.h"
#include "engine/events/event.h"
#include "engine/render/object.h"

namespace Structures::Screen::Gameplay::Cursor
{
	namespace Render
	{
		using namespace Engine::Render::Object;

		namespace Components
		{
			struct NoteScore : Object
			{
				int64_t end_time;

				static void add_animation(
					const std::weak_ptr<NoteScore>& object, 
					Engine::Events::Action::Buffer& action_buffer, 
					const int64_t& earned_time);

				explicit NoteScore(const Memory& skin,
					const Core::Type::Game::Gameplay::NoteScore& score);
			};
			
		}

		struct Cursor : Collection
		{
		protected:
			std::shared_ptr<Object> cursor;
			std::shared_ptr<Collection> note_scores;

		public:
			std::weak_ptr<const Engine::Events::Timing::Timer> timer;
			const Memory* skin;

			void check_and_add_score_shown(
				const Engine::Events::Event::Internal::Buffer::TypeViewer& scoring_events,
				Engine::Events::Action::Buffer& action_buffer) const;

			explicit Cursor(const Memory& skin, std::weak_ptr<const Engine::Events::Timing::Timer> timer);
		};
	}
}