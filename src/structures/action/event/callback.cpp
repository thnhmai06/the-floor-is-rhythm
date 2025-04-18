#include "structures/action/event/callback.h" // Header
#include "utilities.hpp"

namespace Structures::Action::Event
{
	// Callback
	bool Callback::is_active(const int64_t& current_time)
	{
		return Utilities::Math::in_range(start_time, end_time, current_time);
	}

	// CallbackBuffer
	void CallbackBuffer::execute(const int64_t& current_time)
	{
		for (auto event_ptr = events.begin(); event_ptr != events.end(); ++event_ptr)
			if (*event_ptr && event_ptr->get()->is_active(current_time))
				event_ptr->get()->execute(current_time);
			else events.erase(event_ptr);
	}
}
