#pragma once
#include "structures/game/mapset.h"
#include "structures/game/mapset/hitobject.h"
#include "structures/render/object.h"

namespace Structures::Screen::Gameplay::Mapset
{
	namespace Render
	{
		using namespace Structures::Render::Object;

		namespace Components
		{
			struct Floor final : Collection
			{
				const Game::Beatmap::HitObjects::Floor* object;

				Floor(
					const Game::Beatmap::HitObjects::Floor* floor,
					const Memory& memory,
					const SDL_FPoint& centre_pos);
			};
		}

		struct Mapset : Collection
		{
		protected:
			const Game::Beatmap::TimingPoints::TimingPoints* timing_points;

		public:
			struct RenderScript
			{
				std::weak_ptr<Collection> r_obj;
				bool is_kat;
				size_t left_index, right_index;
			};

			using RenderScripts = std::multimap<int64_t, RenderScript>;

		private:
			RenderScripts render_script;
			Collection left, right;

		public:
			[[nodiscard]] const RenderScripts* get_render_scripts() const;
			void render(const SDL_FPoint& offset) override;
			void set_render_range(const int64_t& current_time);
			void set_current_pos(const int64_t& current_time);

			explicit Mapset(const Memory& memory, const Game::Beatmap::Mapset& mapset);
		};
	}
}
