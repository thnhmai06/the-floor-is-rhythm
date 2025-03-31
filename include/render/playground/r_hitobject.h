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
		const HitObject::HitObject* hit_object = nullptr;

		static RenderObject create_adjacent_object(
			const std::string* skin_name,
			const TextureMemory* memory,
			const float& speed,
			const float& duration,
			const SDL_FPoint& spacing = { 0, 0 },
			const HitObject::HitObject* previous = nullptr
		);
		static RenderObject create_hit_object(
			const std::string* skin_name,
			const TextureMemory* memory,
			const HitObject::HitObject* current,
			const float& speed,
			const float& duration,
			const RenderHitobject* previous = nullptr);
		RenderHitobject() = default;
		RenderHitobject(
			const std::string* skin_name,
			const TextureMemory* memory,
			const HitObject::HitObject* current,
			const float& speed,
			const float& duration,
			const RenderHitobject* previous = nullptr);
	};
	struct RenderFloor final : RenderHitobject
	{
		RenderFloor(
			const HitObject::Floor* floor,
			const TextureMemory* memory,
			const Metadata::CalculatedDifficulty* diff,
			const float& current_timing_velocity = 1,
			const RenderHitobject* previous = nullptr);
	};
	struct RenderSlider final : RenderHitobject
	{
	private:
		static RenderObject create_hit_object(
			const std::string* skin_name,
			const TextureMemory* memory,
			const HitObject::HitObject* current,
			const float& speed,
			const float& duration,
			const RenderHitobject* previous = nullptr);
		static RenderObject create_slider_line(
			const SDL_FRect& previous_dst,
			const float& src_retain_percent,
			bool retain_from_beginning = true,
			const float& dst_width,
			const Template::Game::Direction::Direction& current_direction);
		static RenderObject create_slider_point(
			const Template::Game::Direction::Direction& current_direction);
		static RenderObject create_slider_curve(
			const Template::Game::Direction::Direction& current_direction,
			const Template::Game::Direction::Rotation& rotation);
	public:
		RenderSlider(
			const HitObject::Slider* slider,
			const TextureMemory* memory,
			const Metadata::CalculatedDifficulty* diff,
			const float& current_beatlength,
			const float& current_timing_velocity = 1,
			const RenderHitobject* previous = nullptr);
	};
}