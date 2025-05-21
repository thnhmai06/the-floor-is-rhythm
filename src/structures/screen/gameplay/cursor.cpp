#include "structures/screen/gameplay/cursor.h" // Header
#include "format/skin.h"
#include "config.h"
#include "engine/events/action/render.h"
#include "structures/events/event/internal/gameplay.h"

namespace Structures::Screen::Gameplay::Cursor
{
	namespace Render
	{
		using namespace ::Config::Game::Render::Cursor;
		namespace Components
		{
			NoteScore::NoteScore(
				const Memory& skin,
				const Core::Type::Game::Gameplay::NoteScore& score)
				: Object(skin.find(Format::Skin::Image::HitObject::hit.at(score)), ORIGIN)
			{
				config.alpha.percent = 0;
				set_render_size(::Config::Game::Render::Cursor::NoteScore::get_size());
			}

			void NoteScore::add_animation(
				const std::weak_ptr<NoteScore>& object,
				Engine::Events::Action::Buffer& action_buffer,
				const int64_t& earned_time)
			{
				using namespace NoteScore;

				const auto it = object.lock();
				const int64_t full_appear_moment = earned_time + TIME_FADE_IN;
				const int64_t disappear_moment = full_appear_moment + TIME_STAY;
				it->end_time = disappear_moment + TIME_FADE_OUT;

				const auto fade_in = std::make_shared<Engine::Events::Action::Render::FadeAction>(
					earned_time, full_appear_moment, EASING_IN, object.lock()->config, 0, 1);
				const auto move_in = std::make_shared<Engine::Events::Action::Render::MoveAction>(
					earned_time, full_appear_moment, EASING_IN, object.lock()->config, get_start_pos(), get_end_pos());
				const auto fade_out = std::make_shared<Engine::Events::Action::Render::FadeAction>(
					disappear_moment, it->end_time, EASING_OUT, object.lock()->config, 1, 0);
				const auto move_out = std::make_shared<Engine::Events::Action::Render::MoveAction>(
					disappear_moment, it->end_time, EASING_OUT, object.lock()->config, get_end_pos(), get_start_pos());
				action_buffer.submit(fade_in);
				action_buffer.submit(move_in);
				action_buffer.submit(fade_out);
				action_buffer.submit(move_out);
			}
		}

		void Cursor::check_and_add_score_shown(
			const Engine::Events::Event::Internal::Buffer::TypeViewer& scoring_events,
			Engine::Events::Action::Buffer& action_buffer) const
		{
			if (const auto locked_timer = Utilities::Pointer::check_weak(timer))
			{
				const auto current_time = locked_timer->get_last_point();

				auto item = note_scores->data.begin();
				while (item != note_scores->data.end())
				{
					if (const auto ptr = std::get_if<std::shared_ptr<Object>>(&*item))
					{
						if (auto it = std::dynamic_pointer_cast<Components::NoteScore>(*ptr))
						{
							if (it->end_time <= current_time)
							{
								it.reset();
								item = note_scores->data.erase(item);
							}
							else ++item;
						}
						else ++item;
					}
					else ++item;
				}

				for (const auto& [time, event] : scoring_events)
				{
					if (const auto event_ptr = Utilities::Pointer::check_weak(event))
					{
						if (const auto it = std::dynamic_pointer_cast<const Events::Event::Internal::Gameplay::Scoring>(event_ptr))
						{
							if (it->score == Core::Type::Game::Gameplay::NoteScore::Skip) continue;

							auto object = std::make_shared<Components::NoteScore>(*skin, it->score);
							note_scores->data.emplace_back(object);
							Components::NoteScore::add_animation(object, action_buffer, time);
						}
					}
				}
			}
		}
		Cursor::Cursor(const Memory& skin, std::weak_ptr<const Engine::Events::Timing::Timer> timer)
			: Collection(), timer(std::move(timer)), skin(&skin)
		{
			using namespace Components;

			//! Body
			cursor = std::make_shared<Object>(skin.find(Format::Skin::Image::Cursor::body), ORIGIN, get_pos());
			cursor->set_render_size(::Config::user_config->gameplay.cursor.get_pixel_size());
			data.emplace_back(cursor);

			//! Note Scores
			note_scores = std::make_shared<Collection>();
			data.emplace_back(note_scores);
		}
	}
}
