#pragma once
#include "structures/render/layers/objects/collection.h"
#include "structures/render/textures/texture.h"

namespace Structures::Render::Objects::Gameplay::HUD
{

		struct HealthBar : PolyObject
		{
			Object* colour_object = nullptr;

			HealthBar(
				const TextureMemory::Item& background,
				const TextureMemory::Item& colour,
				const float& health = 1.0f);

			// Health là uint8_t từ 0 -> 200
			void set_health(const uint8_t& health) const;
		};
}