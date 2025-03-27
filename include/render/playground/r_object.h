#pragma once
#include "game/hitobject.h"
#include "render/texture.h"

namespace RenderObject
{
	struct RenderObject
	{
		virtual ~RenderObject() = default;
		const std::string* name = nullptr;
		TextureRenderConfig config;

		virtual void render(const TextureMemory& memory) const;
	};

	struct RenderFloor final : RenderObject
	{
		const HitObject::Floor* floor = nullptr;

		RenderFloor(const HitObject::Floor* floor, const SDL_FRect* prev_dstrect_config = nullptr);
	};
}