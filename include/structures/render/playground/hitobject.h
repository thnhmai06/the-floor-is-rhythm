#pragma once
#include "structures/game/hitobject.h"
#include "structures/game/metadata.h"
#include "structures/render/object.h"
#include "structures/render/texture.h"

namespace Structures::Render::RenderObjects::Playground
{
	using Render::RenderObjects::RenderObject;

	struct RenderHitObject : RenderObjects
	{
		const GameObjects::HitObjects::HitObject* hit_object = nullptr; // note: không dùng tham chiếu &

		static RenderObject create_adjacent_object(
			const Texture& texture,
			const SDL_FPoint& size,
			const RenderObject& previous,
			float src_width_in_percent = 1,
			bool src_from_beginning = true);
		static RenderObject create_spacing_object(
			const Texture& texture,
			const GameObjects::HitObjects::HitObject& current,
			const float& speed,
			const RenderHitObject* previous = nullptr);

		RenderHitObject() = default;
		RenderHitObject(
			const Texture& texture,
			const GameObjects::HitObjects::HitObject& current,
			const float& speed,
			const RenderHitObject* previous = nullptr);
	};
	struct RenderFloor final : RenderHitObject
	{
		RenderFloor(
			const GameObjects::HitObjects::HitObject& floor_hit_object,
			const TextureMemory& memory,
			const GameObjects::Metadata::CalculatedDifficulty& diff,
			const float& current_timing_velocity = 1,
			const RenderHitObject* previous = nullptr);
	};
	struct RenderSlider final : RenderHitObject
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
			const RenderHitObject* previous = nullptr);
	};
}