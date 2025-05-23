#pragma once
#include "structures/render/object.h"

namespace Structures::Render::Object
{
	struct PercentObject : Object
	{
		SDL_FPoint render_size_when_full;
		const float* percent;
		bool horizontal = true;
		bool vertical = false;

		void update();
		void on_before_render() override;

		explicit PercentObject(
			const Memory::Item& texture,
			const SDL_FPoint& render_size_when_full,
			const float* percent,
			bool horizontal = true,
			bool vertical = false,
			const Types::Render::OriginType& origin_type = Types::Render::OriginType::Centre,
			const SDL_FPoint& render_pos = { 0, 0 });
		explicit PercentObject(
			const Memory::Item& texture,
			const SDL_FPoint& render_size_when_full,
			const float* percent,
			const Config::OriginPoint& custom_origin,
			bool horizontal = true,
			bool vertical = false,
			const SDL_FPoint& render_pos = { 0, 0 });
	};

	struct StaticPercentObject : PercentObject
	{
		float percent = 1;

		explicit StaticPercentObject(
			const Memory::Item& texture,
			const SDL_FPoint& render_size_when_full,
			bool horizontal = true,
			bool vertical = false,
			const Types::Render::OriginType& origin_type = Types::Render::OriginType::Centre,
			const SDL_FPoint& render_pos = { 0, 0 });
		explicit StaticPercentObject(
			const Memory::Item& texture,
			const SDL_FPoint& render_size_when_full,
			const Config::OriginPoint& custom_origin,
			bool horizontal = true,
			bool vertical = false,
			const SDL_FPoint& render_pos = { 0, 0 });
	};
}