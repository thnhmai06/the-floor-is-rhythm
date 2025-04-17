#pragma once
#include <list>
#include <SDL3/SDL_events.h>

namespace Structures::Action::Event::Input
{
	using KeyboardEventList = std::list<SDL_KeyboardEvent>;
}