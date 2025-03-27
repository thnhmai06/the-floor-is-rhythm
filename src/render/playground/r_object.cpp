#include "render/playground/r_object.h" // Header
#include "exceptions.h"
#include "rule/skin.h"
#include "rule/config.h"
#include "utilities.h"

//! RenderObject
void RenderObject::RenderObject::render(const TextureMemory& memory) const
{
	if (!memory.render(*name, config))
		LOG_ERROR(SDL_Exceptions::Texture::SDL_RenderTexture_Failed(*name));
}

//! Playground
//::RenderFloor
RenderObject::Playground::RenderFloor::RenderFloor(
	const HitObject::Floor* floor,
	const Metadata::CalculatedDifficulty* diff,
	const Timing::InheritedPoint* current_inherited_point,
	const RenderHitobject* previous)
{
	name = &Skin_Filename::HitObject::FLOOR;
	hitobject = floor;

	//! Tính toán
	const float current_timing_velocity = current_inherited_point ? (current_inherited_point->velocity) : 1;
	const float velocity = diff->velocity * current_timing_velocity;
	// Tính pos trung tâm
	if (!previous)
	{
		dst.x = ImmutableConfig::HitObject::DEFAULT_POS_X;
		dst.y = ImmutableConfig::HitObject::DEFAULT_POS_Y;
	} else
	{
		dst.x = previous->config.dst_rect->x;
		dst.y = previous->config.dst_rect->y;
		const auto time_delta = static_cast<float>(floor->time - previous->hitobject->end_time);

		switch (floor->direction_jump)
		{
		case HitObject::DirectionJump::NO_ROTATE:
			dst.x += velocity * time_delta;
			break;

		case HitObject::DirectionJump::ROTATE_90:
			dst.y -= velocity * time_delta;
			break;

		case HitObject::DirectionJump::ROTATE_180:
			dst.x -= velocity * time_delta;
			break;

		case HitObject::DirectionJump::ROTATE_270:
			dst.y += velocity * time_delta;
			break;
		}
	}
	// Tính kích cỡ (width)
	dst.w = velocity * diff->od.bad * 2;
}

//::RenderSlider
RenderObject::Playground::RenderSlider::RenderSlider(
	const HitObject::Slider* slider, 
	const Metadata::CalculatedDifficulty* diff, 
	const Timing::UninheritedPoint* current_uninherited_point, 
	const Timing::InheritedPoint* current_inherited_point, 
	const RenderHitobject* previous) : slider(slider)
{
	name = &Skin_Filename::HitObject::Slider::BEGIN;
	hitobject = slider;

}
