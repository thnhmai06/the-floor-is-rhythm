#include "engine/events/condition.h" // Header

namespace Engine::Events::Condition
{
	namespace Internal
	{
		std::shared_ptr<Condition> Condition::clone() const
		{
			return std::make_shared<Condition>(*this);
		}
		bool Condition::is_satisfied(const Event::Internal::EventView& event) const
		{
			if (event.expired()) return false;
			const auto ptr = event.lock();

			if (!allowed_types.contains(ptr->type)) return false;
			if (time_from.has_value() && ptr->time < time_from.value()) return false;
			if (time_to.has_value() && ptr->time > time_to.value()) return false;
			return true;
		}
		Condition::Condition(
			std::set<Event::Internal::EventType> allowed_types, 
			const std::optional<Time>& time_from, 
			const std::optional<Time>& time_to)
			: allowed_types(std::move(allowed_types)), time_from(time_from), time_to(time_to)
		{
		}
	}

	namespace External
	{
		bool Condition::is_satisfied(Event::External::EventView event) const
		{
			if (!allowed_types.contains(static_cast<SDL_EventType>(event.type))) return false;
			if (time_from.has_value() && event.common.timestamp < time_from.value()) return false;
			if (time_to.has_value() && event.common.timestamp > time_to.value()) return false;
			return true;
		}
		Condition::Condition(
			std::set<Event::External::EventType> allowed_types,
			const std::optional<Uint64>& time_from,
			const std::optional<Uint64>& time_to)
			: allowed_types(std::move(allowed_types)), time_from(time_from), time_to(time_to)
		{
		}
	}
}