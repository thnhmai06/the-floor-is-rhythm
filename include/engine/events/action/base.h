#pragma once
#include "engine/events/timing/easing.h"
#include "engine/events/action.h"
#include "utilities.h"
#include "logging/exceptions.h"
#include "logging/logger.h"

namespace Engine::Events::Action
{
	using namespace Timing::Easing;

	template <typename ValueType>
	struct ChangeValueAction : Action {
	private:
		[[nodiscard]] double get_current_value_percent(const int64_t& current_time) const
		{
			const auto [start, end] = get_sequence_time();

			return get_easing_function(easing)(
				Utilities::Math::to_percent(current_time, start, end));
		}
		bool next_sequence()
		{
			if (current_sequence >= static_cast<long>(sequence.size()) - 1) return false;

			on_next_sequence();
			++current_sequence;
			return true;
		}

	protected:
		virtual void on_started() {}
		virtual void on_next_sequence() {}
		virtual void on_finished() {}
		virtual void update(const double& value_percent, const ValueType& from, const ValueType& to) {}

		void work(const int64_t& current_time)
		{
			if (!is_valid(current_time)) return;
			if (!started)
			{
				started = true;
				on_started();
			}
			const auto value_percent = get_current_value_percent(current_time);
			const auto [crr_from, crr_to] = get_current_value();
			update(value_percent, crr_from, crr_to);
		}

	public:
		static constexpr long DEFAULT_SEQUENCE = -1;

		ValueType from, to;
		ValueType* target;
		Easing easing;
		std::vector<ValueType> sequence;
		long current_sequence = DEFAULT_SEQUENCE;

		ChangeValueAction() = default;
		ChangeValueAction(
			const int64_t& start_time,
			const int64_t& end_time,
			const Easing& easing,
			ValueType* target,
			const ValueType& from,
			const ValueType& to,
			const std::vector<ValueType>& sequence = {})
			: from(from), to(to), target(std::move(target)), easing(easing), sequence(sequence)
		{
			this->start_time = start_time;
			const auto base_end = end_time;
			const auto base_duration = base_end - start_time;
			const auto n = static_cast<int64_t>(sequence.size());
			this->end_time = start_time + base_duration * (n + 1);
		}

		[[nodiscard]] std::shared_ptr<Action> clone() const override
		{
			return std::make_shared<ChangeValueAction>(*this);
		}

		void execute(const int64_t& current_time) final
		{
			try
			{
				if (auto sequence_time = get_sequence_time();
					current_time < sequence_time.second)
					work(current_time);
				else {
					while (sequence_time.second <= current_time)
					{
						if (next_sequence())
						{
							sequence_time = get_sequence_time();
							work(current_time);
						}
						else
						{
							work(current_time);
							finished = true;
							on_finished();
							return;
						}

					}
				}
			}
			catch (const std::exception& e)
			{
				LOG_ERROR(Logging::Exceptions::Engine::Events::Action::Action_ChangeValue_Execute_Failed(e));
			}
		}

		[[nodiscard]] bool is_valid(const int64_t& current_time) const override
		{
			return Action::is_valid(current_time) && !target;
		}

		[[nodiscard]] std::pair<int64_t, int64_t> get_sequence_time() const
		{
			const auto n = static_cast<long>(sequence.size());
			if (n == 0) return { start_time, end_time };

			const auto one_sequence_duration = (end_time - start_time) / (n + 1);
			const auto seg_index = std::clamp(current_sequence + 1, 0L, n);
			return { start_time + one_sequence_duration * seg_index, start_time + one_sequence_duration * (seg_index + 1) };
		}

		[[nodiscard]] std::pair<ValueType, ValueType> get_current_value() const
		{
			const auto n = static_cast<long>(sequence.size());

			if (current_sequence < 0)
				return { from, to };
			if (current_sequence == 0)
				return { to, sequence[0] };
			if (current_sequence == 1)
				return { sequence[0], sequence[1] };
			if (current_sequence >= n)
				return { sequence[n - 2], sequence[n - 1] };
			return { sequence[current_sequence - 1], sequence[current_sequence] };
		}
	};

	struct CallbackAction : Action
	{
		std::function<void(const int64_t& current_time)> callback = nullptr;

		[[nodiscard]] std::shared_ptr<Action> clone() const override;
		void execute(const int64_t& current_time) override;

		explicit CallbackAction(const int64_t& time, std::function<void(const int64_t& current_time)> callback);
	};

	//! Chú ý: Không lồng ConditionalAction trong LoopAction
	struct LoopAction final : Action
	{
	protected:
		Buffer::Container callbacks;

	public:
		Buffer::Container* target;
		uint32_t loop_count;

		[[nodiscard]] std::shared_ptr<Action> clone() const override;
		[[nodiscard]] const Buffer::Container& get_callbacks() const;
		Buffer::Container::iterator add_callback(std::shared_ptr<Action> callback);
		void add_callback(const Buffer::Container& callbacks);
		[[nodiscard]] bool is_valid(const int64_t& current_time) const override;
		void execute(const int64_t& current_time) override;

		explicit LoopAction(Buffer::Container* target, const int64_t& start_time, const uint32_t& loop_count = 1);
	};

	struct ConditionalAction final : Action
	{
		using Condition = Condition::Internal::Condition;

	protected:
		Buffer::Container callbacks;

		[[nodiscard]] bool is_satisfied() const;

	public:
		Buffer::Container* target;
		const Event::Internal::Buffer* event_buffer;
		std::shared_ptr<Condition> condition;

		[[nodiscard]] std::shared_ptr<Action> clone() const override;
		[[nodiscard]] const Buffer::Container& get_callbacks() const;
		Buffer::Container::iterator add(std::shared_ptr<Action> callback);
		void add(const Buffer::Container& callbacks);
		void execute(const int64_t& current_time) override;

		explicit ConditionalAction(
			Buffer::Container* target, const Event::Internal::Buffer& event_buffer,
			const int64_t& start_time, const int64_t& end_time,
			std::shared_ptr<Condition> condition);
	};
}
