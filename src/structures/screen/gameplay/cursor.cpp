#include "structures/screen/gameplay/cursor.h" // Header
#include "format/skin.h"
#include "config.h"
#include "structures/events/action/render.hpp"
#include "structures/events/event/playing.h"

namespace Structures::Screen::Gameplay::Cursor
{
	namespace Render
	{
		using namespace ::Config::Game::Render::Cursor;
		namespace Components
		{
			NoteScore::NoteScore(
				const Memory& skin,
				const Types::Game::Gameplay::NoteScore& score)
				: Object(skin.find(Format::Skin::Image::HitObject::hit.at(score)), ORIGIN)
			{
				config.color.a = 0;
				set_render_size(::Config::Game::Render::Cursor::NoteScore::get_size());
			}

			void NoteScore::add_animation(
				const std::weak_ptr<NoteScore>& object,
				Events::Action::Buffer& action_buffer, const int64_t& time_earned)
			{
				using namespace NoteScore;

				const auto it = object.lock();
				const int64_t full_appear_moment = time_earned + TIME_FADE_IN;
				const int64_t disappear_moment = full_appear_moment + TIME_STAY;
				it->end_time = disappear_moment + TIME_FADE_OUT;

				auto fade_in = std::make_shared<Events::Action::Render::FadeAction>(
					time_earned, full_appear_moment, EASING_IN, object, 0, SDL_MAX_ALPHA);
				auto move_in = std::make_shared<Events::Action::Render::MoveAction>(
					time_earned, full_appear_moment, EASING_IN, object, get_start_pos(), get_end_pos());
				auto fade_out = std::make_shared<Events::Action::Render::FadeAction>(
					disappear_moment, it->end_time, EASING_OUT, object, SDL_MAX_ALPHA, 0);
				auto move_out = std::make_shared<Events::Action::Render::MoveAction>(
					disappear_moment, it->end_time, EASING_OUT, object, get_end_pos(), get_start_pos());
				action_buffer.data.emplace(time_earned, fade_in);
				action_buffer.data.emplace(time_earned, move_in);
				action_buffer.data.emplace(disappear_moment, fade_out);
				action_buffer.data.emplace(disappear_moment, move_out);
			}
		}

		void Cursor::check_and_add_score_shown(const Events::Event::Buffer& event_buffer, Events::Action::Buffer& action_buffer, const int64_t& current_time)
		{
			auto item = note_score_list.begin();
			while (item != note_score_list.end())
			{
				if ((*item)->end_time <= current_time)
				{
					item->reset();
					item = note_score_list.erase(item);
				}
				else ++item;
			}

			const auto events = event_buffer.search({ Types::Event::EventID::Scoring }, current_time);
			if (events.empty()) return;
			for (const auto& [time, event] : events.at(Types::Event::EventID::Scoring))
			{
				if (event.expired()) continue;
				const auto& event_ptr = event.lock();
				if (!event_ptr) continue;
				if (const auto it = std::dynamic_pointer_cast<Events::Event::Playing::Scoring>(event_ptr))
				{
					if (it->score == Types::Game::Gameplay::NoteScore::Skip) continue;
					auto object = note_score_list.emplace_back(std::make_shared<Components::NoteScore>(*skin, it->score));
					Components::NoteScore::add_animation(object, action_buffer, time);
				}
			}

			note_scores->data.clear();
			for (const auto& note_score : note_score_list)
				note_scores->data.emplace_back(note_score);
		}
		Cursor::Cursor(const Memory& skin)
			: Collection(), skin(&skin)
		{
			using namespace Components;

			//! Body
			body = std::make_shared<Object>(skin.find(Format::Skin::Image::Cursor::body), ORIGIN, get_pos());
			body->set_render_size(::Config::user_config->gameplay.cursor.get_pixel_size());
			data.emplace_back(body);

			//! Note Scores
			note_scores = std::make_shared<Collection>();
			data.emplace_back(note_scores);
		}
	}
}
