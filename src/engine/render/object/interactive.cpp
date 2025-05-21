#include "engine/render/object/interactive.h" // Header
#include <ranges>
#include <utility>

namespace Engine::Render::Object
{
	//! InteractiveObject
	void InteractiveObject::update()
	{
		if (!on_event || !event_buffer) return;
		if (!allow_when_hide && (!config.visible || config.alpha.percent <= 0)) return;

		for (const auto query = event_buffer->search(condition);
			const auto & events : query | std::views::values)
		{
			for (const auto& event : events | std::views::values)
			{
				on_event(this, event);
			}
		}
	}

	InteractiveObject::InteractiveObject(
		Memory::Item texture,
		const Events::Event::External::Buffer& event_buffer,
		std::shared_ptr<Events::Condition::External::Condition> condition,
		const OriginType& origin_type, const SDL_FPoint& render_pos,
		std::function<void(Object* object, const Events::Event::External::Event& event)> on_event)
		: Object(std::move(texture), origin_type, render_pos),
		event_buffer(&event_buffer), condition(std::move(condition)), on_event(std::move(on_event))
	{
	}

	InteractiveObject::InteractiveObject(
		Memory::Item texture,
		const Events::Event::External::Buffer& event_buffer,
		std::shared_ptr<Events::Condition::External::Condition> condition,
		const Config::OriginPoint& custom_origin, const SDL_FPoint& render_pos,
		std::function<void(Object* object, const Events::Event::External::Event& event)> on_event)
		: Object(std::move(texture), custom_origin, render_pos),
		event_buffer(&event_buffer), condition(std::move(condition)), on_event(std::move(on_event))
	{
	}
}
