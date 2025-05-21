#pragma once
#include <stack>
#include "engine/events/action.h"

namespace Engine::Screen
{
	struct Stack;
	struct Screen
	{
	protected:
		bool active = false;

		bool enabled = true;
		bool finished = false;

	public:
		[[nodiscard]] const bool& get_active() const;
		[[nodiscard]] const bool& get_enabled() const;
		[[nodiscard]] const bool& get_finished() const;
		virtual void on_activate();
		virtual void on_deactivate();
		virtual void update(Stack& screens_stack) = 0;

		virtual ~Screen() = default;
	};

	struct Stack
	{
		using ScreenContainer = std::list<std::shared_ptr<Screen>>;
		using StackContainer = std::stack<ScreenContainer>;

	protected:
		ScreenContainer removing_screens;

	public:
		Events::Event::External::Buffer external_events;
		ScreenContainer sticky;
		StackContainer normal;

		Uint64 update();
	};
}