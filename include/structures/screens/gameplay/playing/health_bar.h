#pragma once
#include "config.h"
#include "structures/render/collection.h"
#include "structures/render/objects/percent.h"

namespace Structures::Screens::Gameplay::Playing::HealthBar
{
	using namespace Structures::Render::Objects;

	namespace Components
	{
		inline SDL_FPoint render_size = {
			Config::GameConfig::Video::LOGICAL_WIDTH * 0.4f,
			Config::GameConfig::Video::LOGICAL_WIDTH * 0.08f,
		}; // 5:1

		struct Background : Object
		{
			explicit Background(const TextureMemory& skin);
		};

		struct Colour : PercentObject
		{
			explicit Colour(const TextureMemory& skin, const float* percent);
		};
	}

	struct HealthBar : Collection
	{
		std::weak_ptr<Components::Background> background;
		std::weak_ptr<Components::Colour> colour;

		HealthBar(const TextureMemory& skin, const float* percent);
	};
}
