#pragma once
#include "structures/game/mapset.h"
#include "structures/game/mapset/hitobject.h"
#include "engine/render/object.h"

namespace Structures::Screen::Gameplay::Mapset
{
	namespace Render
	{
		using namespace Engine::Render::Object;

		namespace Components
		{
			struct Floor final : Collection
			{
				const Game::Beatmap::HitObjects::Floor* object;

			private:
				void add_animation(
					Engine::Events::Action::Buffer& game_buffer, 
					const std::pair<const Engine::Events::Timing::Time, const SDL_FPoint>& from,
					const std::pair<const Engine::Events::Timing::Time, const SDL_FPoint>& to);

			public:
				Floor(
					const Game::Beatmap::HitObjects::Floor& floor,
					const Memory& memory,
					Engine::Events::Action::Buffer& game_buffer,
					const std::pair<const Engine::Events::Timing::Time, const SDL_FPoint>& from,
					const std::pair<const Engine::Events::Timing::Time, const SDL_FPoint>& to);
			};
		}

		struct Mapset : Collection
		{
			using RenderScripts = std::unordered_map<const Game::Beatmap::HitObjects::Floor*, std::weak_ptr<Components::Floor>>;
		protected:
			const Game::Beatmap::TimingPoints::TimingPoints* timing_points;
			RenderScripts render_scripts;

		public:
			[[nodiscard]] const RenderScripts& get_scripts() const;
			explicit Mapset(
				const Memory& skin,
				Engine::Events::Action::Buffer& game_actions,
				const Game::Beatmap::Mapset& mapset);
		};
	}
}
