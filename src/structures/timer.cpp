#include "structures/timer.h" // Header

using Structures::Timer;

void Timer::create_pause_snapshot(const Uint64& current_tick)
{
	total_paused_tick += current_tick - current_pause_snapshot_tick;
	current_pause_snapshot_tick = current_tick;
}

int64_t Timer::get_time(const Uint64& current_tick)
{
	if (paused) create_pause_snapshot(current_tick);
	const auto passed_time = current_tick - start_tick - total_paused_tick;
	return start_time + passed_time;
}
void Timer::pause()
{
	if (!paused)
	{
		current_pause_snapshot_tick = SDL_GetTicks();
		paused = true;
	}
}
void Timer::resume()
{
	if (paused)
	{
		create_pause_snapshot();
		paused = false;
	}
}
void Timer::reset(const Uint64& start_tick)
{
	this->start_tick = start_tick;
	this->total_paused_tick = 0;
	this->current_pause_snapshot_tick = start_tick;
	this->paused = false;
}
