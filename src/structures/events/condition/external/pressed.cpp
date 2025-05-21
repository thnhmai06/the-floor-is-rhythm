#include "structures/events/condition/external/pressed.h" // Header

namespace Structures::Events::Condition::External
{
	namespace External
	{
		//! KeyDownCondition
		bool KeyDownCondition::is_satisfied(Engine::Events::Event::External::EventView event) const
		{
			return Condition::is_satisfied(event) && keys.contains(event.key.scancode);
		}
		KeyDownCondition::KeyDownCondition(std::unordered_set<SDL_Scancode> keys, const Engine::Events::Timing::Time& time_from)
			: Condition({ SDL_EVENT_KEY_DOWN }, time_from), keys(std::move(keys))
		{
		}

		//! KeyPressCondition
		KeyPressCondition::KeyPressCondition(std::unordered_set<SDL_Scancode> keys, const Engine::Events::Timing::Time& time_from)
			: KeyDownCondition(std::move(keys), time_from)
		{
			allowed_types.emplace(SDL_EVENT_KEY_UP);
		}
	}
}