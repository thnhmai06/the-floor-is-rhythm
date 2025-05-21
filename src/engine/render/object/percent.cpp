#include "engine/render/object/percent.h" // Header

namespace Engine::Render::Object
{
	//! PercentObject
	void PercentObject::update()
	{
		if (!percent) return;

		SDL_FPoint render_size = render_size_when_full;
		if (horizontal)
		{
			src_rect_in_percent.w = static_cast<float>(*percent);
			render_size.x = render_size_when_full.x * static_cast<float>(*percent);
		}
		if (vertical)
		{
			src_rect_in_percent.h = static_cast<float>(*percent);
			render_size.y = render_size_when_full.y * static_cast<float>(*percent);
		}
		set_render_size(render_size);
	}
	void PercentObject::on_before_render() { update(); }
	PercentObject::PercentObject(
		const Memory::Item& texture,
		const SDL_FPoint& render_size_when_full,
		const double* percent, const bool horizontal, const bool vertical,
		const OriginType& origin_type, const SDL_FPoint& render_pos)
		: Object(texture, origin_type, render_pos),
		render_size_when_full(render_size_when_full), percent(percent), horizontal(horizontal), vertical(vertical)
	{
	}
	PercentObject::PercentObject(
		const Memory::Item& texture,
		const SDL_FPoint& render_size_when_full,
		const double* percent, const Config::OriginPoint& custom_origin,
		const bool horizontal, const bool vertical,
		const SDL_FPoint& render_pos)
		: Object(texture, custom_origin, render_pos),
		render_size_when_full(render_size_when_full), percent(percent), horizontal(horizontal), vertical(vertical)
	{
	}

	//! StaticPercentObject
	StaticPercentObject::StaticPercentObject(const Memory::Item& texture,
		const SDL_FPoint& render_size_when_full, const bool horizontal, const bool vertical,
		const OriginType& origin_type, const SDL_FPoint& render_pos)
		: PercentObject(texture, render_size_when_full, nullptr, horizontal, vertical, origin_type, render_pos)
	{
		PercentObject::percent = &percent;
		PercentObject::update();
	}
	StaticPercentObject::StaticPercentObject(const Memory::Item& texture,
		const SDL_FPoint& render_size_when_full, const Config::OriginPoint& custom_origin,
		const bool horizontal, const bool vertical, const SDL_FPoint& render_pos)
		: PercentObject(texture, render_size_when_full, nullptr, custom_origin, horizontal, vertical, render_pos)
	{
		PercentObject::percent = &percent;
		PercentObject::update();
	}
}
