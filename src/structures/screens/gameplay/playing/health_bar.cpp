#include "structures/screens/gameplay/playing/health_bar.h" // Header
#include "format/skin.h"

namespace Structures::Screens::Gameplay::Playing::HealthBar
{
	namespace Components
	{
		Background::Background(
			const TextureMemory& skin)
			: Object(skin.find(
				Format::Skin::HealthBar::background),
				Types::Render::OriginType::TOP_LEFT,
				{ 0, 0 })
		{
			set_render_size(render_size);
		}

		Colour::Colour(
			const TextureMemory& skin,
			const float* percent
		)
			: PercentObject(
				skin.find(Format::Skin::HealthBar::colour),
				render_size, percent, true, false,
				Types::Render::OriginType::TOP_LEFT, SDL_FPoint{ 0, 0 })
		{
		}
	}

	HealthBar::HealthBar(const TextureMemory& skin, const float* percent)
	{
		data.reserve(2);
		//! Thứ tự: Background -> Colour

		// Background
		auto c_background = std::make_shared<Components::Background>(skin);
		background = c_background;
		data.emplace_back(std::move(c_background));

		// Colour
		auto c_colour = std::make_shared<Components::Colour>(skin, percent);
		colour = c_colour;
		data.emplace_back(std::move(c_colour));
	}

}