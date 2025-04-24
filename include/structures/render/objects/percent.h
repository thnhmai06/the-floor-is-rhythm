#pragma once
#include "structures/render/object.h"

namespace Structures::Render::Objects
{
	struct PercentObject : Object
	{
		SDL_FPoint render_size_when_full;
		const float* percent;
		bool horizontal = true;
		bool vertical = false;

		void update();

		explicit PercentObject(
			const TextureMemory::Item& texture,
			const SDL_FPoint& render_size_when_full,
			const float* percent,
			bool horizontal = true,
			bool vertical = false,
			const Types::Render::OriginType& origin_type = Types::Render::OriginType::CENTRE,
			const SDL_FPoint& render_pos = { 0, 0 });
		explicit PercentObject(
			const TextureMemory::Item& texture,
			const SDL_FPoint& render_size_when_full,
			const float* percent,
			const Config::OriginPoint& custom_origin,
			bool horizontal = true,
			bool vertical = false,
			const SDL_FPoint& render_pos = { 0, 0 });
	};
}