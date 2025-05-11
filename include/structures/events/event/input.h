#pragma once
#include <list>
#include <unordered_set>
#include <SDL3/SDL_events.h>

namespace Structures::Events::Event::Input
{
	using SdlEvents = std::list<SDL_Event>;
	using KeyboardEvents = std::list<SDL_KeyboardEvent>;
	using MouseEvents = std::list<SDL_MouseButtonEvent>;

	inline const std::unordered_set<Uint32> mouse_event_types
	{
		SDL_EVENT_MOUSE_BUTTON_DOWN,
		SDL_EVENT_MOUSE_BUTTON_UP,
		SDL_EVENT_MOUSE_MOTION,
		SDL_EVENT_MOUSE_ADDED,
		SDL_EVENT_MOUSE_REMOVED,
		SDL_EVENT_MOUSE_WHEEL
	};
	inline const std::unordered_set<Uint32> key_event_types
	{
		SDL_EVENT_KEY_DOWN,
		SDL_EVENT_KEY_UP
	};
}