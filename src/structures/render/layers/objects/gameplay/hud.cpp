#include "structures/render/layers/objects/gameplay/hud.h" // Header
#include "config.h"
#include "utilities.hpp"

namespace Structures::Render::Objects::Gameplay::HUD
{
	HealthBar::HealthBar(const TextureMemory::Item& background, const TextureMemory::Item& colour, const float& health)
	{
		auto background_item = Object{ background, Types::Render::RenderOriginType::TOP_LEFT, { 0, 0 } };
		data.emplace_back(std::make_unique<Object>(std::move(background_item)));

		auto colour_item = Object{ colour, Types::Render::RenderOriginType::TOP_LEFT, { 0, 0 } };
		auto colour_unique = std::make_unique<Object>(std::move(colour_item));
		data.emplace_back(std::move(colour_unique));
		colour_object = std::get_if<ObjectUnique>(&data.back())->get();
	}

	void HealthBar::set_health(const uint8_t& health) const
	{
		constexpr uint8_t MAX_HEALTH = 200;
		constexpr float HEALTH_BAR_FULL_WIDTH = Config::GameConfig::Video::LOGICAL_WIDTH * 0.4f;
		constexpr float HEALTH_BAR_HEIGHT = Config::GameConfig::Video::LOGICAL_HEIGHT * 0.1f;

		const float health_in_percent = std::min(
			static_cast<float>(health) / static_cast<float>(MAX_HEALTH), 1.0f);
		this->colour_object->src_rect_in_percent = { .x = 0, .y = 0, .w = health_in_percent, .h = 1 };
		this->colour_object->set_render_size({ HEALTH_BAR_FULL_WIDTH * health_in_percent, HEALTH_BAR_HEIGHT });
	}
}