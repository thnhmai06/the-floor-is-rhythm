#pragma once
#include "game/hitobject.h"
#include "game/metadata.h"
#include "game/timing.h"
#include "render/texture.h"

namespace RenderObject
{
	struct RenderObject
	{
		const std::string* name = nullptr;
		TextureRenderConfig config;

		virtual void render(const TextureMemory& memory) const;
		virtual ~RenderObject() = default;
	};

	namespace Playground
	{
		struct RenderHitobject : RenderObject
		{
			const HitObject::HitObject* hitobject = nullptr;
			SDL_FRect dst;

			RenderHitobject() { config.dst_rect = &dst; }
		};

		struct RenderFloor final : RenderHitobject
		{
			RenderFloor(const HitObject::Floor* floor,
				const Metadata::CalculatedDifficulty* diff,
				const Timing::InheritedPoint* current_inherited_point = nullptr,
				const RenderHitobject* previous = nullptr);
		};

		struct RenderSlider final : RenderHitobject
		{
			const HitObject::Slider* slider = nullptr;

			RenderSlider(const HitObject::Slider* slider,
				const Metadata::CalculatedDifficulty* diff,
				const Timing::UninheritedPoint* current_uninherited_point,
				const Timing::InheritedPoint* current_inherited_point = nullptr,
				const RenderHitobject* previous = nullptr);
		};
	}
}