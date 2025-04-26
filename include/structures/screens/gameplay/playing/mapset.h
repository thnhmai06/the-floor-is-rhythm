#pragma once
#include <array>
#include "structures/game/beatmap.h"
#include "structures/game/beatmap/hitobject.h"
#include "structures/render/collection.h"
#include "structures/render/texture.h"

namespace Structures::Screens::Gameplay::Playing::Mapset
{
	using namespace Structures::Render::Objects;

	namespace Components
	{
		struct HitObject : PolyObject
		{
			const Game::Beatmap::HitObjects::Floor* hit_object;

			explicit HitObject(const Game::Beatmap::HitObjects::Floor* hit_object);
			explicit HitObject(const std::weak_ptr<const Game::Beatmap::HitObjects::Floor>& hit_object);
		};
		struct Floor final : HitObject
		{
			Floor(
				const Game::Beatmap::HitObjects::Floor* floor,
				const TextureMemory& memory,
				const SDL_FPoint& centre_pos,
				const SDL_FPoint& size);
		};
		struct Slider final : HitObject
		{
		private:
			float add_slider_line(
				const TextureMemory& memory,
				const SDL_FPoint& centre_width_edge_pos,
				const float& full_width,
				const float& percent);
			void add_slider_point(
				const TextureMemory& memory,
				const SDL_FPoint& centre_pos,
				const SDL_FPoint& size);
			void add_slider_begin(
				const TextureMemory& memory,
				const SDL_FPoint& centre_pos,
				const SDL_FPoint& size);
			void add_slider_end(
				const TextureMemory& memory,
				const SDL_FPoint& centre_pos,
				const SDL_FPoint& size);
			void add_slider_reverse_arrow(
				const TextureMemory& memory,
				const SDL_FPoint& size,
				bool back_to_left = false
			);

		public:
			bool to_right;
			SDL_FPoint left_pos, right_pos;
			std::vector<uint32_t> slider_line_index;
			std::vector<uint32_t> slider_point_index;
			uint32_t begin_index, end_index;
			std::array<uint32_t, 2> slider_reverse_arrow_index;

			Slider(
				const Game::Beatmap::HitObjects::Slider* slider,
				const TextureMemory& memory,
				const float& diff_pixel_speed,
				const float& current_beat_length,
				const float& current_timing_velocity,
				const SDL_FPoint& centre_pos,
				bool to_right = true);
		};
	}

	struct Mapset : Collection
	{
		Mapset(const TextureMemory& memory, const Game::Beatmap::Mapset& beatmap);
	};
}