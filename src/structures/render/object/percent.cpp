#include "structures/render/object/percent.h" // Header

namespace Structures::Render::Object
{
	void PercentObject::update()
	{
		if (!percent) return;

		SDL_FPoint render_size = render_size_when_full;
		if (horizontal)
		{
			src_rect_in_percent.w = *percent;
			render_size.x = render_size_when_full.x * *percent;
		}
		if (vertical)
		{
			src_rect_in_percent.h = *percent;
			render_size.y = render_size_when_full.y * *percent;
		}
		set_render_size(render_size);
	}

	void PercentObject::on_before_render()
	{
		update();
	}

	PercentObject::PercentObject(
		const Memory::Item& texture,
		const SDL_FPoint& render_size_when_full,
		const float* percent, const bool horizontal, const bool vertical,
		const Types::Render::OriginType& origin_type, const SDL_FPoint& render_pos)
		: Object(texture, origin_type, render_pos),
		render_size_when_full(render_size_when_full), percent(percent), horizontal(horizontal), vertical(vertical)
	{
		update();
	}
	PercentObject::PercentObject(
		const Memory::Item& texture,
		const SDL_FPoint& render_size_when_full,
		const float* percent, const Config::OriginPoint& custom_origin,
		const bool horizontal, const bool vertical,
		const SDL_FPoint& render_pos)
			: Object(texture, custom_origin, render_pos),
		render_size_when_full(render_size_when_full), percent(percent), horizontal(horizontal), vertical(vertical)
	{
		update();
	}

	StaticPercentObject::StaticPercentObject(const Memory::Item& texture,
		const SDL_FPoint& render_size_when_full, const bool horizontal, const bool vertical,
		const Types::Render::OriginType& origin_type, const SDL_FPoint& render_pos)
	: PercentObject(texture, render_size_when_full, nullptr, horizontal, vertical, origin_type, render_pos)
	{
		PercentObject::percent = &percent;
		update();
	}
	StaticPercentObject::StaticPercentObject(const Memory::Item& texture, 
		const SDL_FPoint& render_size_when_full, const Config::OriginPoint& custom_origin,
		const bool horizontal, const bool vertical, const SDL_FPoint& render_pos)
	: PercentObject(texture, render_size_when_full, nullptr, custom_origin, horizontal, vertical, render_pos)
	{
		PercentObject::percent = &percent;
		update();
	}
}
