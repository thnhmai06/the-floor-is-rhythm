#include "structures/screen/gameplay/health.h" // Header
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
					::Config::GameConfig::Render::Health::ORIGIN, render_pos)
			{
				set_render_size(render_size);
			}

			Colour::Colour(
				const Memory& skin,
				const float* percent
			)
				: PercentObject(
					skin.find(Format::Skin::Image::HealthBar::colour),
					render_size, percent, true, false,
					::Config::GameConfig::Render::Health::ORIGIN, render_pos)
			{
			}
		}

		Health::Health(const Memory& memory, const float* percent)
		{
			data.reserve(2);
			//! Thứ tự: Background -> Colour

			// Background
			auto c_background = std::make_shared<Components::Background>(memory);
			background = c_background;
			data.emplace_back(std::move(c_background));

			// Colour
			auto c_colour = std::make_shared<Components::Colour>(memory, percent);
			colour = c_colour;
			data.emplace_back(std::move(c_colour));
		}

	}

	namespace Logic
	{
		// ::Core::Health
		bool Health::update(const Types::Game::Gameplay::NoteScore& note_score, const unsigned long& current_combo)
		{
			if (*is_paused) return true;
			value = std::clamp(value + hp_diff->get_gained_health(note_score, current_combo), 0.0f, 1.0f);
			return *no_fail || !(value <= 0 && (note_score == Types::Game::Gameplay::NoteScore::Miss));
		}
		const float* Health::get_health() const { return &value; }
		Health::Health(
			const Game::Beatmap::Mapset& mapset,
			const bool* is_paused, const bool* no_fail) :
			hp_diff(&mapset.calculated_difficulty->hp_drain_rate), is_paused(is_paused), no_fail(no_fail)
		{
		}
	}
}