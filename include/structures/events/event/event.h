#pragma once
#include "structures/type.hpp"
#include <set>
#include <optional>

namespace Structures::Events::Event
{
	struct Event
	{
		int64_t time;

		virtual Types::Event::EventID get_id() noexcept;

		virtual ~Event() = default;
	};

	struct Buffer
	{
		using ID_CONTAINER = std::multimap<int64_t, std::shared_ptr<Event>>;
		using CONTAINER = std::unordered_map<Types::Event::EventID, ID_CONTAINER>;

	protected:
		CONTAINER data;

	public:
		[[nodiscard]] const CONTAINER& get_data() const noexcept;
		[[nodiscard]] std::unordered_map<Types::Event::EventID, std::multimap<int64_t, std::weak_ptr<Event>>>
		search(const std::set<Types::Event::EventID>& ids,
			const std::optional<int64_t>& time_from = std::nullopt, 
			const std::optional<int64_t>& time_to = std::nullopt) const;
		void add(const int64_t& time, std::shared_ptr<Event> event);
		void clear() noexcept;
	};
}