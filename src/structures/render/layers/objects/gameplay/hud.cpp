#include "structures/render/layers/objects/gameplay/hud.h" // Header
#include "config.h"
#include "utilities.h"

using namespace Structures::Render::RenderObjects::Gameplay;
using namespace Components;

HealthBar::HealthBar(const TextureMemory::Item& background, const TextureMemory::Item& colour, const float& health)
{
	push_back(RenderObject{ background, Template::Render::RenderOriginType::TOP_LEFT, {0, 0} });
	push_back(RenderObject{ colour, Template::Render::RenderOriginType::TOP_LEFT, {0, 0} });
	this->colour_object = &back();
}

void HealthBar::set_health(const uint8_t& health) const
{
	constexpr uint8_t MAX_HEALTH = 200;
	constexpr float HEALTH_BAR_FULL_WIDTH = GameConfig::Video::LOGICAL_WIDTH * 0.4f;
	constexpr float HEALTH_BAR_HEIGHT = GameConfig::Video::LOGICAL_HEIGHT * 0.1f;

	const float health_in_percent = Utilities::Math::min_float(
		static_cast<float>(health) / static_cast<float>(MAX_HEALTH), 1);
	this->colour_object->src_rect_in_percent = { .x = 0, .y = 0, .w = health_in_percent, .h = 1 };
	this->colour_object->set_scale_fixed({ HEALTH_BAR_FULL_WIDTH * health_in_percent, HEALTH_BAR_HEIGHT });
}
