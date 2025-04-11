#pragma once
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <SDL3/SDL_scancode.h>

template<>
struct std::hash<SDL_Scancode>
{
	size_t operator()(const SDL_Scancode& key) const noexcept;
};

namespace Structures::Action
{
	using KeyHashTable = std::unordered_set<SDL_Scancode>;
	struct KeyQueue : std::queue<std::pair<bool, SDL_Scancode>>
	{
	protected:
		using BASE = queue;

	public:
		bool is_catching = true;
		std::list<bool(*)(const SDL_Scancode&)> filter;

		void push(const SDL_Scancode& key, bool is_press_event);
	};
	struct KeyStatus
	{
		bool is_pressed = false;
		bool is_hold = false;
	};
	struct KeyObserver : std::unordered_map<SDL_Scancode, KeyStatus>
	{
	protected:
		KeyHashTable hold_only_keys;
		std::queue<SDL_Scancode> need_update_to_hold_only;

	public:
		void update();
		void update(const KeyQueue& queue);
	};

	struct PlayingKeyQueue : KeyQueue
	{
		PlayingKeyQueue();
	};
}

