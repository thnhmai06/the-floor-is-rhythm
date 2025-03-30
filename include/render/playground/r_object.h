#pragma once
#include <SDL3/SDL_rect.h>
#include "game/hitobject.h"
#include "game/metadata.h"
#include "render/texture.h"

namespace RenderObjects::Playground
{
	struct RenderHitobject : RenderObject
	{
		const HitObject::HitObject* hitobject = nullptr;

		RenderHitobject(
			const float& velocity,
			const float& duration,
			const RenderHitobject* previous);
	};

	struct RenderFloor final : RenderHitobject
	{
		RenderFloor(
			const HitObject::Floor* floor,
			const Metadata::CalculatedDifficulty* diff,
			const RenderHitobject* previous = nullptr,
			const float& current_timing_velocity = 1);
	};

	struct RenderSlider final : RenderHitobject
	{
	private:
		[[nodiscard]] RenderHitobject create_slider_end() const;

	public:
		RenderHitobject slider_end;

		struct SliderComponents : std::vector<RenderObject>
		{
		private:
			[[nodiscard]] RenderObject create_slider_line(
				const SDL_FRect& previous_dst,
				const float& src_retain_percent,
				bool retain_from_beginning = true,
				const float& dst_width,
				const Direction::Direction& current_direction) const;
			[[nodiscard]] RenderObject create_slider_point(
				const Direction::Direction& current_direction) const;
			[[nodiscard]] RenderObject create_slider_curve(
				const Direction::Direction& current_direction,
				const Direction::Rotation& rotation) const;

		public:
			SliderComponents(
				const HitObject::Slider* slider,
				const SDL_FRect* start_dst,
				const float& current_beatlength,
				const float& velocity);
		} components;

		RenderSlider(
			const HitObject::Slider* slider,
			const Metadata::CalculatedDifficulty* diff,
			const float& current_beatlength,
			const RenderHitobject* previous = nullptr,
			const float& current_timing_velocity = 1);
	};
}