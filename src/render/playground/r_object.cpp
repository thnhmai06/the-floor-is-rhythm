#include "render/playground/r_object.h" // Header
#include "rule/skin.h"
#include "rule/config.h"

static SDL_FRect* calculate_next_render_hitobject(
	SDL_FRect* dst,
	const HitObject::DirectionJump& direction_jump,
	const float& velocity,
	const float& duration,
	const float& time_delta,
	const RenderObjects::Playground::RenderHitobject* previous)
{
	// Tính vị trí đặt
	if (!previous)
	{
		dst->x = ImmutableConfig::HitObject::DEFAULT_POS_X;
		dst->y = ImmutableConfig::HitObject::DEFAULT_POS_Y;
	}
	else
	{
		if (!previous->tail)
		{
			dst->x = previous->config.dst_rect->x;
			dst->y = previous->config.dst_rect->y;
		}
		else
		{
			dst->x = previous->tail->x;
			dst->y = previous->tail->y;
		}

		switch (direction_jump)
		{
		case HitObject::DirectionJump::NO_ROTATE:
			dst->x += velocity * time_delta;
			break;

		case HitObject::DirectionJump::ROTATE_90:
			dst->y -= velocity * time_delta;
			break;

		case HitObject::DirectionJump::ROTATE_180:
			dst->x -= velocity * time_delta;
			break;

		case HitObject::DirectionJump::ROTATE_270:
			dst->y += velocity * time_delta;
			break;
		}
	}
	// Tính kích cỡ (width)
	dst->w = velocity * duration;

	return dst;
}

//! RenderObjects::Playground
// ::RenderFloor
RenderObjects::Playground::RenderFloor::RenderFloor(
	const HitObject::Floor* floor,
	const Metadata::CalculatedDifficulty* diff,
	const RenderHitobject* previous,
	const float& current_timing_velocity)
{
	name = &SkinFormat::HitObject::FLOOR;
	hitobject = floor;
	tail = config.dst_rect.get();

	const float velocity = diff->velocity * current_timing_velocity;
	const auto time_delta = static_cast<float>(floor->time - previous->hitobject->end_time);
	calculate_next_render_hitobject(config.dst_rect.get(), floor->direction_jump, velocity,
		diff->od.bad * 2, time_delta, previous);
}

//::RenderSlider
RenderObjects::Playground::RenderSlider::RenderSlider(
	const HitObject::Slider* slider,
	const Metadata::CalculatedDifficulty* diff,
	const float& current_beatlength,
	const RenderHitobject* previous,
	const float& current_timing_velocity)
{
	name = &SkinFormat::HitObject::Slider::BEGIN;
	hitobject = slider;

	const float velocity = diff->velocity * current_timing_velocity;
	const auto time_delta = static_cast<float>(slider->time - previous->hitobject->end_time);
	calculate_next_render_hitobject(config.dst_rect.get(), slider->direction_jump, velocity,
		diff->od.bad * 2, time_delta, previous);

	//TODO: draw components
}