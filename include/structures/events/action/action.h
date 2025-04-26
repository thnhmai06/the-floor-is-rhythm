#pragma once
#include <map>
#include "structures/events/condition/condition.h"

namespace Structures::Events::Action
{
	struct Action
	{
		bool started = false;
		bool finished = false;
		int64_t start_time, end_time;

		virtual void execute(const int64_t& current_time);
		[[nodiscard]] virtual bool is_valid(const int64_t& current_time) const;

		virtual ~Action() = default;
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

	//! Chú ý: Các lệnh bên dưới không nên lồng nhau
	//! (Loop trong loop hoặc Loop trong condition và ngược lại)
	struct LoopAction final : Action
	{
	protected:
		Buffer::CONTAINER callbacks;

	public:
		Buffer::CONTAINER* target;
		uint32_t loop_count;

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
		std::unique_ptr<Condition::Condition> condition;

		[[nodiscard]] const Buffer::CONTAINER& get_callbacks() const;
		Buffer::CONTAINER::iterator add(std::shared_ptr<Action> callback);
		void add(const Buffer::CONTAINER& callbacks);
		void execute(const int64_t& current_time) override;

		explicit ConditionalAction(
			Buffer::CONTAINER* target, const Event::Buffer* location,
			const int64_t& start_time, const int64_t& end_time,
			std::unique_ptr<Condition::Condition> condition);
	};
}
