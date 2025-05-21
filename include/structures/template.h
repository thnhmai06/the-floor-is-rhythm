#pragma once
#include "engine/events/condition.h"

namespace Structures::Template
{
	namespace Events
	{
		namespace Condition
		{
			namespace External
			{
				inline const auto press_mouse = std::set{ SDL_EVENT_MOUSE_BUTTON_UP, SDL_EVENT_MOUSE_BUTTON_DOWN };
			}
		}
	}
}