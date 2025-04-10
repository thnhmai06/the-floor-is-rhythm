#pragma once
#include <cstdint>
#include <SDL3/SDL_timer.h>

namespace Structures
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
		void pause();
		void resume();
		void reset(const Uint64& start_tick = SDL_GetTicks());

		explicit Timer(const int64_t& start_time = 0, const Uint64& start_tick = SDL_GetTicks())
		: start_tick(start_tick), current_pause_snapshot_tick(start_tick), start_time(start_time)
		{
		}
	};
}