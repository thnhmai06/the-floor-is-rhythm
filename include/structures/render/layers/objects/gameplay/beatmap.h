#pragma once
#include "structures/game/beatmap/hitobject.h"
#include "structures/game/beatmap/metadata.h"
#include "structures/render/layers/objects/collection.h"
#include "structures/render/textures/texture.h"
#include "structures/game/beatmap/timing.h"

namespace Structures::Render::Objects::Gameplay::Beatmap
{
	namespace Components
	{
		struct HitObject : PolyObject
		{
			const Game::Beatmap::HitObjects::HitObject* hit_object = nullptr; // note: không dùng tham chiếu &

			static Object create_spacing_object(
				const TextureMemory::Item& texture,
				const Game::Beatmap::HitObjects::HitObject& current,
				const float& speed,
				const HitObject* previous = nullptr);

			HitObject() = default;
			HitObject(
				const TextureMemory::Item& texture,
				const Game::Beatmap::HitObjects::HitObject& current,
				const float& speed,
				const HitObject* previous = nullptr);
		};
		struct Floor final : HitObject
		{
			Floor(
				const Game::Beatmap::HitObjects::HitObject& floor_hit_object,
				const TextureMemory& memory,
				const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
				const float& current_timing_velocity = 1,
				const HitObject* previous = nullptr);
		};
		struct Slider final : HitObject
		{
		private:
			static Object create_adjacent_object(
				const TextureMemory::Item& texture,
				const SDL_FPoint& size,
				const Object& previous,
				float src_width_in_percent = 1,
				bool src_from_beginning = true);
			static Object create_slider_point(
				const TextureMemory::Item& texture,
				const Object& previous);
		public:
			Slider(
				const Game::Beatmap::HitObjects::HitObject& slider_object,
				const TextureMemory& memory,
				const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
				const float& current_beat_length,
				const float& current_timing_velocity = 1,
				const HitObject* previous = nullptr);
		};
	}

	struct Collection final : Objects::Collection
	{
	protected:
		using BASE = Collection;

	public:
		Collection(const TextureMemory& memory,
			const Game::Beatmap::HitObjects::HitObjects& hit_objects,
			const Game::Beatmap::Metadata::CalculatedDifficulty& difficulty,
			const Game::Beatmap::TimingPoints::TimingPoints& timing_points);
	};
}