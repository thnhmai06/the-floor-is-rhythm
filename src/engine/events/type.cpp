#include "engine/events/type.h" // Header

namespace Engine::Events
{
	namespace Event
	{
		namespace Internal
		{
			Event::Event(const Timing::Time& time, const EventType& type)
				: time(time), type(type)
			{
			}

		}
	}
}