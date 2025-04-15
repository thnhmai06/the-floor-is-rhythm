#pragma once
#include "structures/game/beatmap/beatmap.h"
#include "structures/game/beatmap/hitobject.h"
#include "structures/game/beatmap/metadata.h"
#include "structures/render/layers/objects/collection.h"
#include "structures/render/textures/texture.h"

namespace Structures::Render::Objects::Gameplay::Beatmap
{
	namespace Components
	{
		struct HitObject : PolyObject
		{
			const Game::Beatmap::HitObjects::HitObject* hit_object = nullptr;
		};
		struct Floor final : HitObject
		{
			Floor(
				const Game::Beatmap::HitObjects::HitObject& floor,
				const TextureMemory& memory,
				const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
				const float& current_timing_velocity = 1,
				bool no_rotation = false,
				const HitObject* previous = nullptr);
		};
		struct Slider final : HitObject
		{
		private:
			static Object create_slider_line(
				const TextureMemory& memory,
				const SDL_FPoint& size,
				const Object& previous,
				float src_width_in_percent = 1,
				bool from_centre_of_previous = false);
			static Object create_slider_point(
				const TextureMemory& memory,
				const Object& previous);
			static Object create_slider_begin(
				const Game::Beatmap::HitObjects::HitObject& slider,
				const TextureMemory& memory,
				const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
				const float& current_timing_velocity = 1,
				const HitObject* previous = nullptr);
		public:
			Slider(
				const Game::Beatmap::HitObjects::HitObject& slider,
				const TextureMemory& memory,
				const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
				const float& current_beat_length,
				const float& current_timing_velocity,
				bool no_rotation = false,
				const HitObject* previous = nullptr);
		};
	}

	struct Collection : Objects::Collection
	{
		Collection(const TextureMemory& memory, const Game::Beatmap::Beatmap& beatmap);
	};
}