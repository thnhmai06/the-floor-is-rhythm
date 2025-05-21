#include "structures/screen/gameplay/health.h" // Header
#include "engine/events/timing/easing.h"
#include "format/skin.h"

namespace Structures::Screen::Gameplay::Health
{
	namespace Render
	{
		namespace Components
		{
			Background::Background(
				const Memory& skin)
				: Object(skin.find(
					Format::Skin::Image::HealthBar::background),
					::Config::Game::Render::Health::ORIGIN, ::Config::Game::Render::Health::POS)
			{
				set_render_size(::Config::Game::Render::Health::get_size());
			}

			void Colour::update()
			{
				if (const auto locked_timer = Utilities::Pointer::check_weak(timer))
				{
					const auto current_time = locked_timer->get_last_point();

					if (!last_changed.has_value())
					{
						last_changed.emplace(current_time, *real_percent);
						percent = *real_percent;
					}
					else
					{
						const auto progress = static_cast<float>(Engine::Events::Timing::Easing::get_easing_function(::Config::Game::Render::Health::EASING)(
							Utilities::Math::to_percent(
								current_time,
								last_changed->first,
								last_changed->first + ::Config::Game::Render::Health::CHANGE_TIME,
								true)));
						percent = Utilities::Math::to_value(progress, last_changed->second, *real_percent);
						if (!Utilities::Math::is_equal(*real_percent, percent))
						{
							last_changed->first = current_time;
							last_changed->second = *real_percent;
						}
					}
				}
				StaticPercentObject::update();
			}
			Colour::Colour(const Memory& skin, std::weak_ptr<const Engine::Events::Timing::Timer> timer, const float* real_percent)
				: StaticPercentObject(
					skin.find(Format::Skin::Image::HealthBar::colour),
					::Config::Game::Render::Health::get_size(), true, false,
					::Config::Game::Render::Health::ORIGIN, ::Config::Game::Render::Health::POS),
				real_percent(real_percent), timer(std::move(timer))
			{
			}
		}

		Health::Health(const Memory& memory, const float& percent, std::weak_ptr<const Engine::Events::Timing::Timer> timer)
		{
			//! Thứ tự: Background -> Colour

			// Background
			background = std::make_shared<Components::Background>(memory);
			data.emplace_back(background);

			// Colour
			colour = std::make_shared<Components::Colour>(memory, std::move(timer), &percent);
			data.emplace_back(colour);
		}
	}

	namespace Logic
	{
		// ::Core::Health
		bool Health::update(const Core::Type::Game::Gameplay::NoteScore& note_score, const unsigned long& current_combo)
		{
			if (*is_paused) return true;
			value = std::clamp(value + hp_diff->get_gained_health(note_score, current_combo), 0.0f, 1.0f);
			return *no_fail || !(value <= 0 && (note_score == Core::Type::Game::Gameplay::NoteScore::Miss));
		}

		const float& Health::get_health() const { return value; }
		void Health::reset() { value = 1.0f; }
		Health::Health(
			const Game::Beatmap::Mapset& mapset,
			const bool* is_paused, const bool* no_fail) :
			hp_diff(&mapset.calculated_difficulty->hp_drain_rate), is_paused(is_paused), no_fail(no_fail)
		{
		}

	}
}
