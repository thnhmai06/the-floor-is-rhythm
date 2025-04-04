#pragma once
#include <SDL3/SDL_rect.h>
#include "game/hitobject.h"
#include "game/metadata.h"
#include "render/object.h"
#include "render/texture.h"

namespace RenderObjects::Playground
{
	struct RenderHitobject : RenderObjects
	{
		const GameObjects::HitObjects::HitObject* hit_object = nullptr; // note: không dùng tham chiếu &

		static RenderObject create_object_on_pos(
			const Texture& texture,
			const SDL_FPoint& pos,
			const SDL_FPoint& size);
		static RenderObject create_adjacent_object(
			const Texture& texture,
			const float& speed,
			const float& duration,
			const RenderObject* previous,
			float src_width_in_percent = 1,
			bool src_from_beginning = true);
		static RenderObject create_spacing_object(
			const Texture& texture,
			const GameObjects::HitObjects::HitObject& current,
			const float& speed,
			const float& duration,
			const RenderHitobject* previous = nullptr);

		RenderHitobject() = default;
		RenderHitobject(
			const Texture& texture,
			const GameObjects::HitObjects::HitObject& current,
			const float& speed,
			const float& duration,
			const RenderHitobject* previous = nullptr);
	};
	struct RenderFloor final : RenderHitobject
	{
		RenderFloor(
			const GameObjects::HitObjects::HitObject& floor_object,
			const TextureMemory& memory,
			const GameObjects::Metadata::CalculatedDifficulty& diff,
			const float& current_timing_velocity = 1,
			const RenderHitobject* previous = nullptr);
	};
	struct RenderSlider final : RenderHitobject
	{
	private:
		static RenderObject create_slider_point(
			const Texture& texture,
			const RenderObject& previous);
	public:
		RenderSlider(
			const GameObjects::HitObjects::HitObject& slider_object,
			const TextureMemory& memory,
			const GameObjects::Metadata::CalculatedDifficulty& diff,
			const float& current_beatlength,
			const float& current_timing_velocity = 1,
			const RenderHitobject* previous = nullptr);
	};
}