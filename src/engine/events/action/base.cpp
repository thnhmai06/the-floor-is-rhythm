#include "engine/events/action/base.h" // Header
#include <ranges>

namespace Engine::Events::Action
{
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
	CallbackAction::CallbackAction(const int64_t& time, std::function<void(const int64_t& current_time)> callback)
		: callback(std::move(callback))
	{
		this->start_time = time;
	}

	//! LoopAction
	std::shared_ptr<Action> LoopAction::clone() const
	{
		return std::make_shared<LoopAction>(*this);
	}
	const Buffer::Container& LoopAction::get_callbacks() const { return callbacks; }
	Buffer::Container::iterator LoopAction::add_callback(std::shared_ptr<Action> callback)
	{
		end_time = std::max(end_time, start_time + (callback->end_time) * loop_count);
		return callbacks.emplace(callback->start_time, std::move(callback));
	}
	void LoopAction::add_callback(const Buffer::Container& callbacks)
	{
		for (const auto& callback : callbacks | std::views::values)
			add_callback(callback);
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
	LoopAction::LoopAction(Buffer::Container* target, const int64_t& start_time, const uint32_t& loop_count)
		: target(target), loop_count(loop_count)
	{
		this->start_time = start_time;
	}
	bool ConditionalAction::is_satisfied() const
	{
		const auto it = condition->clone();
		it->time_from = start_time;
		it->time_to = end_time;

		const auto events = event_buffer->search(it);
		return (!events.empty());
	}

	//! ConditionalAction
	std::shared_ptr<Action> ConditionalAction::clone() const
	{
		return std::make_shared<ConditionalAction>(*this);
	}
	const Buffer::Container& ConditionalAction::get_callbacks() const { return callbacks; }
	Buffer::Container::iterator ConditionalAction::add(std::shared_ptr<Action> callback)
	{
		end_time = std::max(end_time, callback->end_time);
		return callbacks.emplace(callback->start_time, std::move(callback));
	}
	void ConditionalAction::add(const Buffer::Container& callbacks)
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
		Buffer::Container* target, const Event::Internal::Buffer& event_buffer,
		const int64_t& start_time, const int64_t& end_time, std::shared_ptr<Condition> condition) :
		target(target), event_buffer(&event_buffer), condition(std::move(condition))
	{
		this->start_time = start_time;
		this->end_time = end_time;
	}
}