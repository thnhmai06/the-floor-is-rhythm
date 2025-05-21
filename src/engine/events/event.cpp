#include "engine/events/event.h" // Header

namespace Engine::Events::Event
{
	namespace Internal
	{
		//! Buffer
		bool Buffer::contains(const std::weak_ptr<const Condition::Internal::Condition>& condition) const
		{
			const auto& it = condition.lock();
			const auto& types = it->allowed_types;
			const auto& time_from = it->time_from;
			const auto& time_to = it->time_to;

			for (const auto& type : types)
			{
				const auto& type_container = data.find(type);
				if (type_container == data.end()) continue;
				const auto& events = type_container->second;

				TypeContainer::const_iterator from, to;
				if (time_from.has_value())
					from = events.lower_bound(time_from.value());
				else from = events.begin();
				if (time_to.has_value())
				{
					to = events.upper_bound(time_to.value());
					if (to != events.begin()) --to;
				}
				else to = events.end();

				for (auto ptr = from; ptr != to; ++ptr)
				{
					if (it->is_satisfied(ptr->second))
						return true;
				}
			}
			return false;
		}
		Buffer::Viewer Buffer::search(const std::weak_ptr<const Condition::Internal::Condition>& condition) const
		{
			const auto& it = condition.lock();
			const auto& types = it->allowed_types;
			const auto& time_from = it->time_from;
			const auto& time_to = it->time_to;

			Viewer result{};
			for (const auto& type : types)
			{
				const auto& type_container = data.find(type);
				if (type_container == data.end()) continue;
				const auto& events = type_container->second;

				TypeContainer::const_iterator from, to;
				if (time_from.has_value())
					from = events.lower_bound(time_from.value());
				else from = events.begin();
				if (time_to.has_value())
				{
					to = events.upper_bound(time_to.value());
					if (to != events.begin()) --to;
				}
				else to = events.end();

				for (auto ptr = from; ptr != to; ++ptr)
				{
					if (it->is_satisfied(ptr->second))
						result[type].emplace(ptr->first, ptr->second);
				}
			}
			return result;
		}
		Buffer::TypeContainer::iterator Buffer::submit(std::shared_ptr<Event> event)
		{
			if (!event) return {};
			return data[event->type].emplace(event->time, std::move(event));
		}
		void Buffer::clear_old(const Time& current_time)
		{
			auto itr = data.begin();
			while (itr != data.end())
			{
				if (itr->second.empty())
				{
					itr = data.erase(itr);
					continue;
				}

				auto head = itr->second.upper_bound(current_time);
				if (head == itr->second.begin()) continue;
				itr->second.erase(itr->second.begin(), std::prev(head));
				if (itr->second.empty()) itr = data.erase(itr);
				else ++itr;
			}
		}
		void Buffer::clear() noexcept
		{
			data.clear();
		}
	}
	namespace External
	{
		bool Buffer::contains(const std::weak_ptr<const Condition::External::Condition>& condition) const
		{
			const auto& it = condition.lock();
			const auto& types = it->allowed_types;
			const auto& time_from = it->time_from;
			const auto& time_to = it->time_to;

			for (const auto& type : types)
			{
				const auto& type_container = data.find(type);
				if (type_container == data.end()) continue;
				const auto& events = type_container->second;

				TypeContainer::const_iterator from, to;
				if (time_from.has_value())
					from = events.lower_bound(time_from.value());
				else from = events.begin();
				if (time_to.has_value())
				{
					to = events.upper_bound(time_to.value());
					if (to != events.begin()) --to;
				}
				else to = events.end();

				for (auto ptr = from; ptr != to; ++ptr)
				{
					if (it->is_satisfied(ptr->second))
						return true;
				}
			}
			return false;
		}
		Buffer::Viewer Buffer::search(const std::weak_ptr<const Condition::External::Condition>& condition) const
		{
			const auto& it = condition.lock();
			const auto& types = it->allowed_types;
			const auto& time_from = it->time_from;
			const auto& time_to = it->time_to;

			Viewer result{};
			for (const auto& type : types)
			{
				const auto& type_container = data.find(type);
				if (type_container == data.end()) continue;
				const auto& events = type_container->second;

				TypeContainer::const_iterator from, to;
				if (time_from.has_value())
					from = events.lower_bound(time_from.value());
				else from = events.begin();
				if (time_to.has_value())
				{
					to = events.upper_bound(time_to.value());
					if (to != events.begin()) --to;
				}
				else to = events.end();

				for (auto ptr = from; ptr != to; ++ptr)
				{
					if (it->is_satisfied(ptr->second))
						result[type].emplace(ptr->first, ptr->second);
				}
			}
			return result;
		}
		Buffer::Container& Buffer::scan()
		{
			if (clear_when_scan) clear();
			bool is_scanning = true;

			Event event;
			while (SDL_PollEvent(&event))
			{
				if (filter && !filter(is_scanning, event))
					continue;
				data[static_cast<SDL_EventType>(event.type)].emplace(event.common.timestamp, event);
				if (!is_scanning) break;
			}
			return data;
		}
		void Buffer::clear_old(const Time& current_time)
		{
			auto itr = data.begin();
			while (itr != data.end())
			{
				if (itr->second.empty())
				{
					itr = data.erase(itr);
					continue;
				}

				auto head = itr->second.upper_bound(current_time);
				if (head == itr->second.begin()) continue;
				itr->second.erase(itr->second.begin(), std::prev(head));
				if (itr->second.empty()) itr = data.erase(itr);
				else ++itr;
			}
		}
		void Buffer::clear() noexcept
		{
			data.clear();
		}
	}
}