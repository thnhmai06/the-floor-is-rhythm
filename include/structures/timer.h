#pragma once
#include <cstdint>
#include <optional>
#include <SDL3/SDL_timer.h>

namespace Structures::Timer
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