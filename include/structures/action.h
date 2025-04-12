#pragma once
#include <optional>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_timer.h>

template<>
struct std::hash<SDL_Scancode>
{
	size_t operator()(const SDL_Scancode& key) const noexcept;
};

namespace Structures::Action
{
	namespace Key
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
			void update(KeyQueue& queue);
		};

		struct PlayingKeyQueue : KeyQueue
		{
			PlayingKeyQueue();
		};
	}
	namespace Time
	{
		struct Timer
		{
		protected:
			Uint64 start_tick = 0;
			Uint64 total_paused_tick = 0;
			Uint64 current_pause_snapshot_tick = 0;
			bool paused = false;

			void create_pause_snapshot(const Uint64& current_tick = SDL_GetTicks());

		public:
			int64_t start_time = 0;

			[[nodiscard]] bool is_paused() const { return paused; }
			[[nodiscard]] int64_t get_time(const Uint64& current_tick = SDL_GetTicks());
			void pause(const Uint64& current_tick = SDL_GetTicks());
			void resume();
			void reset(bool autostart = false, const Uint64& start_tick = SDL_GetTicks(), const std::optional<int64_t>& start_time = std::nullopt);

			explicit Timer(const int64_t& start_time = 0, const Uint64& start_tick = SDL_GetTicks());
		};
	}
}

