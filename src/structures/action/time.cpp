#include "structures/action/time.h" // Header

namespace Structures::Action::Time
{
	Uint64 Timer::get_current_tick()
	{
		return SDL_GetTicks();
	}
	// Timer
	void Timer::create_pause_snapshot(const Uint64& current_tick)
	{
		total_paused_tick += current_tick - current_pause_snapshot_tick;
		current_pause_snapshot_tick = current_tick;
	}
	int64_t Timer::get_time(const Uint64& current_tick)
	{
		if (paused) create_pause_snapshot(current_tick);
		const auto passed_time = current_tick - start_tick - total_paused_tick;
		return start_time + static_cast<int64_t>(passed_time);
	}
	void Timer::pause(const Uint64& current_tick)
	{
		if (paused) return;
		current_pause_snapshot_tick = current_tick;
		paused = true;
	}
	void Timer::resume()
	{
		if (!paused) return;
		create_pause_snapshot();
		paused = false;
	}
	void Timer::reset(const bool autostart, const Uint64& start_tick, const std::optional<int64_t>& start_time)
	{
		if (start_time.has_value()) this->start_time = start_time.value();
		this->start_tick = start_tick;
		this->total_paused_tick = 0;
		this->current_pause_snapshot_tick = start_tick;
		this->paused = false;
		if (!autostart) pause(start_tick);
	}
	Timer::Timer(const int64_t& start_time, const Uint64& start_tick)
		: start_tick(start_tick), current_pause_snapshot_tick(start_tick), start_time(start_time)
	{
		pause(start_tick);
	}
}