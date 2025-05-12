#include "structures/events/action/action.h" // Header
#include <ranges>
#include "utilities.hpp"
#include "structures/events/action/render.hpp"

namespace Structures::Events::Action
{
	std::shared_ptr<Action> Action::clone() const
	{
		return std::make_shared<Action>(*this);
	}
	//! BASE
	void Action::execute(const int64_t& current_time) { started = true; }
	bool Action::is_valid(const int64_t& current_time) const { return !finished && start_time <= current_time; }

	//! Buffer
	void Buffer::execute(const int64_t& current_time)
	{
		if (data.empty()) return;

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

	//! CallbackAction
	std::shared_ptr<Action> CallbackAction::clone() const
	{
		return std::make_shared<CallbackAction>(*this);
	}
	void CallbackAction::execute(const int64_t& current_time)
	{
		if (!is_valid(current_time)) return;
		if (!started) started = true;
		if (callback) callback(current_time);
		if (current_time >= end_time) finished = true;
	}
	CallbackAction::CallbackAction(const int64_t& start_time, std::function<void(const int64_t& current_time)> callback)
		: callback(std::move(callback))
	{
	}

	//! LoopAction
	std::shared_ptr<Action> LoopAction::clone() const
	{
		return std::make_shared<LoopAction>(*this);
	}
	const Buffer::CONTAINER& LoopAction::get_callbacks() const { return callbacks; }
	Buffer::CONTAINER::iterator LoopAction::add(std::shared_ptr<Action> callback)
	{
		end_time = std::max(end_time,  start_time + (callback->end_time) * loop_count);
		return callbacks.emplace(callback->start_time, std::move(callback));
	}
	void LoopAction::add(const Buffer::CONTAINER& callbacks)
	{
		for (const auto& callback : callbacks | std::views::values)
			add(callback);
	}
	bool LoopAction::is_valid(const int64_t& current_time) const
	{
		return target && Action::is_valid(current_time);
	}
	void LoopAction::execute(const int64_t& current_time)
	{
		if (!is_valid(current_time)) return;

		started = true;
		const auto duration = (end_time - start_time) / loop_count;
		for (uint32_t count = 1; count <= loop_count; ++count)
		{
			for (const auto& callback : callbacks | std::views::values)
			{
				auto new_callback = callback->clone();
				new_callback->start_time = start_time + callback->start_time + duration * (count - 1);
				new_callback->end_time = start_time + callback->end_time + duration * (count - 1);

				target->emplace(new_callback->start_time, std::move(new_callback));
			}
		}
		finished = true;
	}
	LoopAction::LoopAction(Buffer::CONTAINER* target, const int64_t& start_time, const uint32_t& loop_count)
		: target(target), loop_count(loop_count)
	{
		this->start_time = start_time;
	}

	//! ConditionalAction
	bool ConditionalAction::is_satisfied() const
	{
		for (const auto events = events_location->search(condition->get_allowed_event(), start_time, end_time); 
			const auto& id_events : events | std::views::values)
		{
			for (const auto& event : id_events | std::views::values)
			{
				if (condition->is_satisfied(event.lock().get()))
					return true;
			}
		}
		return false;
	}
	std::shared_ptr<Action> ConditionalAction::clone() const
	{
		return std::make_shared<ConditionalAction>(*this);
	}
	const Buffer::CONTAINER& ConditionalAction::get_callbacks() const { return callbacks; }
	Buffer::CONTAINER::iterator ConditionalAction::add(std::shared_ptr<Action> callback)
	{
		end_time = std::max(end_time, callback->end_time);
		return callbacks.emplace(callback->start_time, std::move(callback));
	}

	void ConditionalAction::add(const Buffer::CONTAINER& callbacks)
	{
		for (const auto& callback : callbacks | std::views::values)
			add(callback);
	}
	void ConditionalAction::execute(const int64_t& current_time)
	{
		if (finished || current_time < start_time) return;

		started = true;
		if (end_time <= current_time)
		{
			finished = true;
			return;
		}
		if (is_satisfied())
		{
			for (const auto& callback : callbacks | std::views::values)
			{
				std::shared_ptr<Action> new_callback = callback;
				new_callback->start_time += current_time;
				new_callback->end_time += current_time;

				target->emplace(new_callback->start_time, std::move(new_callback));
			}
		}
	}
	ConditionalAction::ConditionalAction(
		Buffer::CONTAINER* target, const Event::Buffer* location,
		const int64_t& start_time, const int64_t& end_time, 
		std::shared_ptr<Condition::Condition> condition)
		: target(target), events_location(location), condition(std::move(condition))
	{
		this->start_time = start_time;
		this->end_time = end_time;
	}
}
