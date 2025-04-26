#include "structures/events/event/event.h" // Header

namespace Structures::Events::Event
{
	//! Event
	Types::Event::EventID Event::get_id() noexcept
	{
		return Types::Event::EventID::None;
	}
	const Buffer::CONTAINER& Buffer::get_data() const noexcept { return data; }

	std::unordered_map<Types::Event::EventID, std::multimap<int64_t, std::weak_ptr<Event>>>
	Buffer::search(
		const std::set<Types::Event::EventID>& ids,
		const std::optional<int64_t>& time_from, const std::optional<int64_t>& time_to) const
	{
		std::unordered_map<Types::Event::EventID, std::multimap<int64_t, std::weak_ptr<Event>>> result {};
		for (const auto& id: ids)
		{
			const auto& category = data.find(id);
			if (category == data.end()) continue;
			const auto& events = category->second;

			ID_CONTAINER::const_iterator from, to;
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
				result[id].emplace(ptr->first, ptr->second);
		}
		return result;
	}

	//! Buffer
	void Buffer::add(const int64_t& time, std::shared_ptr<Event> event)
	{
		data[event->get_id()].emplace(time, std::move(event));
	}
	void Buffer::clear() noexcept
	{
		data.clear();
	}
}
