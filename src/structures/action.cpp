#include "structures/action.h" // Header
#include "config.h"

inline size_t std::hash<SDL_Scancode>::operator()(const SDL_Scancode& key) const noexcept
{
	return static_cast<size_t>(key);
}

using namespace Structures::Action;
// ::KeyQueue
void KeyQueue::push(const SDL_Scancode& key, const bool is_press_event)
{
	if (!is_catching) return;
	for (const auto& func : filter)
		if (!func(key)) return;
	BASE::push({ is_press_event, key });
}
void KeyObserver::update()
{
	while (!need_update_to_hold_only.empty())
	{
		const auto key = need_update_to_hold_only.front();
		need_update_to_hold_only.pop();

		if (hold_only_keys.contains(key))
			at(key).is_pressed = false;
	}
}
void KeyObserver::update(const KeyQueue& queue)
{
	update();

	for (const auto& [is_press_event, key] : queue)
	{
		if (is_press_event)
		{
			if (hold_only_keys.find(key)) continue;
			insert_or_assign(key, { true, true });
			hold_only_keys.insert(key);
			need_update_to_hold_only.push(key);
		}
		else
		{
			insert_or_assign(key, { false, false });
			hold_only_keys.erase(key);
		}
	}
}

// ::PlayingKeyQueue
PlayingKeyQueue::PlayingKeyQueue()
{
	filter.emplace_back(UserConfig::KeyBinding::Click::is_click);
	filter.emplace_back(UserConfig::KeyBinding::Direction::is_direction);
}