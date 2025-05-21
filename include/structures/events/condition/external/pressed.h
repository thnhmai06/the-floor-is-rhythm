#pragma once
#include <unordered_set>
#include "engine/events/condition.h"

namespace Structures::Events::Condition::External
{
	using Engine::Events::Condition::External::Condition;

	namespace External
	{
		struct KeyDownCondition : Condition
		{
			std::unordered_set<SDL_Scancode> keys;

			[[nodiscard]] bool is_satisfied(Engine::Events::Event::External::EventView event) const override;

			explicit KeyDownCondition(std::unordered_set<SDL_Scancode> keys, const Engine::Events::Timing::Time& time_from);
		};

		struct KeyPressCondition : KeyDownCondition
		{
			explicit KeyPressCondition(std::unordered_set<SDL_Scancode> keys, const Engine::Events::Timing::Time& time_from);
		};
	}
}