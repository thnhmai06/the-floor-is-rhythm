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
		// Lưu vị trí cuối cùng của hitobject trước đó; dùng SDL_FRect* thay vì SDL_FPoint* để tránh phải tạo thêm SDL_FPoint
		SDL_FRect* tail = nullptr;

		RenderHitobject() { config.dst_rect = std::make_unique<SDL_FRect>(); }
	};

	struct RenderFloor final : RenderHitobject
	{
		RenderFloor(const HitObject::Floor* floor,
			const Metadata::CalculatedDifficulty* diff,
			const RenderHitobject* previous = nullptr,
			const float& current_timing_velocity = 1);
	};

	struct RenderSlider final : RenderHitobject
	{
		std::vector<RenderHitobject> components;

		RenderSlider(const HitObject::Slider* slider,
			const Metadata::CalculatedDifficulty* diff,
			const float& current_beatlength,
			const RenderHitobject* previous = nullptr,
			const float& current_timing_velocity = 1);
	};
}