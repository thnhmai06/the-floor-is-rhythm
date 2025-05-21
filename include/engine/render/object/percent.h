#pragma once
#include "engine/render/object.h"

namespace Engine::Render::Object
{
	struct PercentObject : Object
	{
		SDL_FPoint render_size_when_full;
		const double* percent;
		bool horizontal = true;
		bool vertical = false;

	protected:
		virtual void update();
		void on_before_render() override;

	public:
		explicit PercentObject(
			const Memory::Item& texture,
			const SDL_FPoint& render_size_when_full,
			const double* percent,
			bool horizontal = true,
			bool vertical = false,
			const OriginType& origin_type = OriginType::Centre,
			const SDL_FPoint& render_pos = { 0, 0 });
		explicit PercentObject(
			const Memory::Item& texture,
			const SDL_FPoint& render_size_when_full,
			const double* percent,
			const OriginPoint& custom_origin,
			bool horizontal = true,
			bool vertical = false,
			const SDL_FPoint& render_pos = { 0, 0 });
	};

	struct StaticPercentObject : PercentObject
	{
		double percent = 1;

		explicit StaticPercentObject(
			const Memory::Item& texture,
			const SDL_FPoint& render_size_when_full,
			bool horizontal = true,
			bool vertical = false,
			const OriginType& origin_type = OriginType::Centre,
			const SDL_FPoint& render_pos = { 0, 0 });
		explicit StaticPercentObject(
			const Memory::Item& texture,
			const SDL_FPoint& render_size_when_full,
			const OriginPoint& custom_origin,
			bool horizontal = true,
			bool vertical = false,
			const SDL_FPoint& render_pos = { 0, 0 });
	};
}