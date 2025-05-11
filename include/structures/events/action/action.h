#pragma once
#include <functional>
#include <map>
#include "structures/events/condition/condition.h"

namespace Structures::Events::Action
{
	struct Action
	{
		bool started = false;
		bool finished = false;
		int64_t start_time, end_time;

		[[nodiscard]] virtual std::shared_ptr<Action> clone() const;
		virtual void execute(const int64_t& current_time);
		[[nodiscard]] virtual bool is_valid(const int64_t& current_time) const;

		Action() = default;
		virtual ~Action() = default;
		Action(const Action&) = default;
		Action& operator=(const Action&) = default;
	};

	// Đây là Buffer, không phải kho chứa Action!
	// Nếu cần kho chứa Action (kiểu dạng đóng gói),
	// hãy xem LoopAction: link:include\structures\events\action\action.h:struct%20LoopAction
	struct Buffer
	{
		using CONTAINER = std::multimap<int64_t, std::shared_ptr<Action>>;

		CONTAINER data;

		void execute(const int64_t& current_time);
	};

	struct CallbackAction : Action
	{
		std::function<void(const int64_t& current_time)> callback = nullptr;

		[[nodiscard]] std::shared_ptr<Action> clone() const override;
		void execute(const int64_t& current_time) override;

		explicit CallbackAction(const int64_t& start_time, std::function<void(const int64_t& current_time)> callback);
	};
	//! Chú ý: Các lệnh bên dưới không nên lồng nhau
	//! (Loop trong loop hoặc Loop trong Condition và ngược lại)
	struct LoopAction final : Action
	{
	protected:
		Buffer::CONTAINER callbacks;

	public:
		Buffer::CONTAINER* target;
		uint32_t loop_count;

		[[nodiscard]] std::shared_ptr<Action> clone() const override;
		[[nodiscard]] const Buffer::CONTAINER& get_callbacks() const;
		Buffer::CONTAINER::iterator add(std::shared_ptr<Action> callback);
		void add(const Buffer::CONTAINER& callbacks);
		[[nodiscard]] bool is_valid(const int64_t& current_time) const override;
		void execute(const int64_t& current_time) override;

		explicit LoopAction(Buffer::CONTAINER* target, const int64_t& start_time, const uint32_t& loop_count = 1);
	};

	struct ConditionalAction final : Action
	{
	protected:
		Buffer::CONTAINER callbacks;

		[[nodiscard]] bool is_satisfied() const;
	public:
		Buffer::CONTAINER* target;
		const Event::Buffer* events_location;
		std::shared_ptr<Condition::Condition> condition;

		[[nodiscard]] std::shared_ptr<Action> clone() const override;
		[[nodiscard]] const Buffer::CONTAINER& get_callbacks() const;
		Buffer::CONTAINER::iterator add(std::shared_ptr<Action> callback);
		void add(const Buffer::CONTAINER& callbacks);
		void execute(const int64_t& current_time) override;

		explicit ConditionalAction(
			Buffer::CONTAINER* target, const Event::Buffer* location,
			const int64_t& start_time, const int64_t& end_time,
			std::shared_ptr<Condition::Condition> condition);
	};
}
