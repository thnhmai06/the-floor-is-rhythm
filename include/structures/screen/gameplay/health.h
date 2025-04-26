#pragma once
#include "config.h"
#include "structures/render/object.h"
#include "structures/render/object/percent.h"
#include "structures/game/mapset.h"

namespace Structures::Screen::Gameplay::Health
{
	namespace Render
	{
		using namespace Structures::Render::Object;

		namespace Components
		{
			static constexpr SDL_FPoint render_size =
			{
				Config::GameConfig::Render::Health::SIZE_W,
				Config::GameConfig::Render::Health::SIZE_H,
			};
			static constexpr SDL_FPoint render_pos = 
			{
				::Config::GameConfig::Render::Health::POS_X,
				::Config::GameConfig::Render::Health::POS_Y
			};

				struct Background : Object
			{
				explicit Background(const Memory& skin);
			};

			struct Colour : PercentObject
			{
				explicit Colour(const Memory& skin, const float* percent);
			};
		}

		struct Health : Collection
		{
			std::weak_ptr<Components::Background> background;
			std::weak_ptr<Components::Colour> colour;

			Health(const Memory& memory, const float* percent);
		};
	}

	namespace Logic
	{
		struct Health
		{
		protected:
			const Game::Beatmap::Metadata::CalculatedDifficulty::HPDrainRate* hp_diff;
			const bool* is_paused;
			const bool* no_fail;

		private:
			float value = 1;

		public:
			[[nodiscard]] const float* get_health() const;
			bool update(const Types::Game::Gameplay::NoteScore& note_score, const unsigned long& current_combo); // -> return: người chơi có fail không?

			explicit Health(const Game::Beatmap::Mapset& mapset, const bool* is_paused, const bool* no_fail);
		};
	}
}
