#include "engine/events/action.h" // Header

#include "utilities.h"

namespace Engine::Events::Action
{
	//! BASE
	const bool& Action::get_started() const { return started; }
	const bool& Action::get_finished() const { return finished; }
	std::shared_ptr<Action> Action::clone() const
	{
		return std::make_shared<Action>(*this);
	}
	void Action::execute(const int64_t& current_time) { started = true; }
	bool Action::is_valid(const int64_t& current_time) const { return !finished && start_time <= current_time; }

	//! Buffer
	Buffer::Container::iterator Buffer::submit(std::shared_ptr<Action> action)
	{
		if (!action) return {};
		return data.emplace(action->start_time, std::move(action));
	}
	void Buffer::execute()
	{
		if (data.empty()) return;

		Timing::Time current_time;
		if (const auto locked_timer = Utilities::Pointer::check_weak(timer))
			current_time = locked_timer->get_last_point();
		else current_time = static_cast<Timing::Time>(SDL_GetTicks());

		auto it = data.begin();
		while (it != data.end() && it->first <= current_time)
		{
			const auto& action = it->second;

			action->execute(current_time);
			if (action->finished)
				it = data.erase(it);
			else ++it;
		}
	}
	Buffer::Buffer(std::weak_ptr<const Timing::Timer> timer)
		: timer(std::move(timer))
	{
	}
}
