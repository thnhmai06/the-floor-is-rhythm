#pragma once
#include "structures/events/time/time.h"
#include "structures/events/action/action.h"

namespace Core::Resources
{
	namespace Event
	{
		inline Structures::Events::Time::Timer runtime{ 0, false };
		inline Structures::Events::Action::Buffer action_buffer;
	}
}