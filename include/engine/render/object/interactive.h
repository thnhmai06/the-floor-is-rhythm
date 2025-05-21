#pragma once
#include "engine/render/object.h"
#include "engine/events/event.h"

namespace Engine::Render::Object
{
	struct InteractiveObject : Object
	{
		bool allow_when_hide = false;
		const Events::Event::External::Buffer* event_buffer;
		std::shared_ptr<Events::Condition::External::Condition> condition;
		std::function<void(Object* object, const Events::Event::External::Event& event)> on_event;

		virtual void update();

		explicit InteractiveObject(
			Memory::Item texture,
			const Events::Event::External::Buffer& event_buffer,
			std::shared_ptr<Events::Condition::External::Condition> condition,
			const OriginType& origin_type = OriginType::Centre,
			const SDL_FPoint& render_pos = { 0, 0 },
			std::function<void(Object* object, const Events::Event::External::Event& event)> on_event = nullptr);
		explicit InteractiveObject(
			Memory::Item texture,
			const Events::Event::External::Buffer& event_buffer,
			std::shared_ptr<Events::Condition::External::Condition> condition,
			const Config::OriginPoint& custom_origin,
			const SDL_FPoint& render_pos = { 0, 0 },
			std::function<void(Object* object, const Events::Event::External::Event& event)> on_event = nullptr);
	};
}