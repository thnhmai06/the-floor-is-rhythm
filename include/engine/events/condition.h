#pragma once
#include <optional>
#include <set>
#include "engine/events/type.h"

namespace Engine::Events::Condition
{
	using Timing::Time;

	namespace Internal
	{
		struct Condition
		{
			std::set<Event::Internal::EventType> allowed_types;
			std::optional<Time> time_from;
			std::optional<Time> time_to;

			[[nodiscard]] virtual std::shared_ptr<Condition> clone() const;
			[[nodiscard]] virtual bool is_satisfied(const Event::Internal::EventView& event) const;

			Condition() = default;
			Condition(
				std::set<Event::Internal::EventType> allowed_types, 
				const std::optional<Time>& time_from = std::nullopt, 
				const std::optional<Time>& time_to = std::nullopt);
			virtual ~Condition() = default;
		};
	}

	namespace External
	{
		struct Condition
		{
			std::set<Event::External::EventType> allowed_types;
			std::optional<Uint64> time_from;
			std::optional<Uint64> time_to;

			[[nodiscard]] virtual bool is_satisfied(Event::External::EventView event) const;

			Condition() = default;
			Condition(
				std::set<Event::External::EventType> allowed_types,
				const std::optional<Uint64>& time_from = std::nullopt,
				const std::optional<Uint64>& time_to = std::nullopt);
			virtual ~Condition() = default;
		};
	}
}