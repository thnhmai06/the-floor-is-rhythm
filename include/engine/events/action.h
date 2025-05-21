#pragma once
#include <map>
#include "engine/events/event.h"
#include "timing/time.h"

namespace Engine::Events::Action
{
	struct Buffer;
	struct Action
	{
	protected:
		bool started = false;
		bool finished = false;

		friend Buffer;
	public:
		int64_t start_time, end_time;
		[[nodiscard]] const bool& get_started() const;
		[[nodiscard]] const bool& get_finished() const;
		[[nodiscard]] virtual std::shared_ptr<Action> clone() const;
		virtual void execute(const int64_t& current_time);
		[[nodiscard]] virtual bool is_valid(const int64_t& current_time) const;

		Action() = default;
		virtual ~Action() = default;
		Action& operator=(const Action&) = default;
	};

	// Đây là Buffer, không phải kho chứa Action!
	// Nếu cần kho chứa Action (kiểu dạng đóng gói),
	// hãy xem LoopAction: link:include\engine\events\action\base.h:struct%20LoopAction%20final
	struct Buffer
	{
		using Container = std::multimap<Timing::Time, std::shared_ptr<Action>>;

		std::weak_ptr<const Timing::Timer> timer;
		Container data;

		Container::iterator submit(std::shared_ptr<Action> action);
		void execute();

		explicit Buffer(std::weak_ptr<const Timing::Timer> timer = std::weak_ptr<const Timing::Timer>());
	};
}
