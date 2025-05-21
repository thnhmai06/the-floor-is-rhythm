#pragma once
#include "engine/render/object/interactive.h"

namespace Structures::Render::Object
{
	using namespace Engine::Render::Object;
	using Engine::Render::OriginType;
	using ExternalEventBuffer = Engine::Events::Event::External::Buffer;
	using ExternalEvent = Engine::Events::Event::External::Event;
	using ExternalCondition = Engine::Events::Condition::External::Condition;

	struct MousePressObject : InteractiveObject
	{
	protected:
		struct PressedOnObject : ExternalCondition
		{
			const Object* object;

			[[nodiscard]] bool is_satisfied(Engine::Events::Event::External::EventView event) const override;

			explicit PressedOnObject(const Object& object);
		};

	public:
		MousePressObject(
			Memory::Item texture,
			const ExternalEventBuffer& event_buffer,
			const OriginType& origin_type = OriginType::Centre,
			const SDL_FPoint& render_pos = { 0, 0 },
			std::function<void(Object* object, const ExternalEvent& event)> on_event = nullptr);
		MousePressObject(
			Memory::Item texture,
			const ExternalEventBuffer& event_buffer,
			const OriginPoint& custom_origin,
			const SDL_FPoint& render_pos = { 0, 0 },
			std::function<void(Object* object, const ExternalEvent& event)> on_event = nullptr);
	};
}