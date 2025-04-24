#pragma once
#include <list>
#include <memory>

namespace Structures::Action::Event
{
	struct Callback
	{
		int64_t start_time, end_time;

		virtual bool is_active(const int64_t& current_time);
		virtual void execute(const int64_t& current_time) = 0;
		[[nodiscard]] int64_t get_relative_time(const int64_t& current_time) const;
		[[nodiscard]] int64_t get_duration() const;

		virtual ~Callback() = default;
	};
	using CallbackUnique = std::unique_ptr<Callback>;


	struct CallbackBuffer
	{
		// đây là CallbackBuffer, không phải kho chứa Callback!
	protected:
		using BASE = std::list<CallbackUnique>;

	public:
		std::list<CallbackUnique> events;

		void execute(const int64_t& current_time);

		virtual ~CallbackBuffer() = default;
	};
}
