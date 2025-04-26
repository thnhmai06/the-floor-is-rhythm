#pragma once
#include <set>
#include <structures/events/event/event.h>

namespace Structures::Events::Condition
{
	struct Condition
	{
		[[nodiscard]] virtual std::set<Types::Event::EventID> get_allowed_event() const { return {}; }
		virtual bool is_satisfied(const Event::Event* event) const { return false; }

		virtual ~Condition() = default;
	};
}