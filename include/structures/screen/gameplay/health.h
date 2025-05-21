#pragma once
#include "config.h"
#include "engine/render/object/percent.h"
#include "structures/game/mapset.h"

namespace Structures::Screen::Gameplay::Health
{
	namespace Render
	{
		using namespace Engine::Render::Object;

		namespace Components
		{
			struct Background : Object
			{
				explicit Background(const Memory& skin);
			};
			struct Colour : StaticPercentObject
			{
			protected:
				std::optional<std::pair<Engine::Events::Timing::Time, float>> last_changed;
				const float* real_percent;

			public:
				std::weak_ptr<const Engine::Events::Timing::Timer> timer;
				void update() override;

				explicit Colour(const Memory& skin, std::weak_ptr<const Engine::Events::Timing::Timer> timer, const float* real_percent);
			};
		}

		struct Health : Collection
		{
			std::shared_ptr<Components::Background> background;
			std::shared_ptr<Components::Colour> colour;

			explicit Health(const Memory& memory, const float& percent, std::weak_ptr<const Engine::Events::Timing::Timer> timer);
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
			[[nodiscard]] const float& get_health() const;
			bool update(const Core::Type::Game::Gameplay::NoteScore& note_score, const unsigned long& current_combo); // -> return: người chơi có fail không?
			void reset();

			explicit Health(
				const Game::Beatmap::Mapset& mapset, 
				const bool* is_paused, const bool* no_fail);
		};
	}
}
