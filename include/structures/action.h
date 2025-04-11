#pragma once
#include <queue>
#include "config.h"

namespace Structures
{
	struct KeyQueue : std::queue<int>
	{
	protected:
		using BASE = std::queue<int>;

	public:
		bool is_catching = false;
		void push(const int& key)
		{
			if (!is_catching) return;
			BASE::push(key);
		}
	};
	struct PlayingKeyQueue : KeyQueue
	{
		void push(const int& key)
		{
			if (UserConfig::KeyBinding::Direction::is_direction(key) || UserConfig::KeyBinding::Click::is_click(key))
				KeyQueue::push(key);
		}
	};
}
