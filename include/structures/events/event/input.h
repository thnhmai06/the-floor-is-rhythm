#pragma once
#include <list>
#include <SDL3/SDL_events.h>

namespace Structures::Events::Event::Input
{
	using SdlEvents = std::list<SDL_Event>;
	using KeyboardEvents = std::list<SDL_KeyboardEvent>;
}