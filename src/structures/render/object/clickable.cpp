#include "structures/render/object/clickable.h" // Header
#include "structures/template.h"

namespace Structures::Render::Object
{
	//! MousePressObject
	// ::PressedOnObject
	bool MousePressObject::PressedOnObject::is_satisfied(Engine::Events::Event::External::EventView event) const
	{
		if (!Condition::is_satisfied(event)) return false;

		const auto& mouse_button_event = event.button;
		return Utilities::Render::is_point_in_rect({ mouse_button_event.x, mouse_button_event.y }, object->get_sdl_dst_rect());
	}
	MousePressObject::PressedOnObject::PressedOnObject(const Object& object)
		: object(&object)
	{
		allowed_types = Template::Events::Condition::External::press_mouse;
	}

	// ::
	MousePressObject::MousePressObject(
		Memory::Item texture, const ExternalEventBuffer& event_buffer,
		const OriginType& origin_type, const SDL_FPoint& render_pos,
		std::function<void(Object* object, const ExternalEvent& event)> on_event)
		: InteractiveObject(std::move(texture), event_buffer,
			std::make_shared<PressedOnObject>(*this),
			origin_type, render_pos, std::move(on_event))
	{
	}
	MousePressObject::MousePressObject(
		Memory::Item texture,
		const ExternalEventBuffer& event_buffer,
		const OriginPoint& custom_origin,
		const SDL_FPoint& render_pos,
		std::function<void(Object* object, const ExternalEvent& event)> on_event)
		: InteractiveObject(std::move(texture), event_buffer,
			std::make_shared<PressedOnObject>(*this),
			custom_origin, render_pos, std::move(on_event))
	{
	}
}
