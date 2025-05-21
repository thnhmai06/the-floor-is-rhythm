#pragma once
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>
#include "engine/events/condition.h"

namespace Engine::Events::Event
{
	using Timing::Time;

	namespace Internal
	{
		struct Buffer
		{
			using TypeContainer = std::multimap<Time, EventCon>;
			using Container = std::unordered_map<EventType, TypeContainer>;
			using TypeViewer = std::multimap<Time, EventView>;
			using Viewer = std::unordered_map<EventType, TypeViewer>;

			Container data;

			[[nodiscard]] bool contains(const std::weak_ptr<const Condition::Internal::Condition>& condition) const;
			[[nodiscard]] Viewer search(const std::weak_ptr<const Condition::Internal::Condition>& condition) const;
			TypeContainer::iterator submit(std::shared_ptr<Event> event);
			void clear_old(const Time& current_time);
			void clear() noexcept;
		};
	}

	namespace External
	{
		struct Buffer
		{
			using TypeContainer = std::multimap<Time, Event>;
			using Container = std::unordered_map<EventType, TypeContainer>;
			using TypeViewer = std::multimap<Time, EventView>;
			using Viewer = std::unordered_map<EventType, TypeViewer>;

			bool clear_when_scan = true;
			std::function<bool(bool& is_scanning, const Event& event)> filter = nullptr;
			Container data;

			[[nodiscard]] bool contains(const std::weak_ptr<const Condition::External::Condition>& condition) const;
			[[nodiscard]] Viewer search(const std::weak_ptr<const Condition::External::Condition>& condition) const;
			Container& scan();
			void clear_old(const Time& current_time);
			void clear() noexcept;
		};
	}
}
