#include "engine/events/timing/time.h" // Header

namespace Engine::Events::Timing
{
	// Timer
	void Timer::create_pause_snapshot(const std::optional<Uint64>& current_tick)
	{
		const Uint64 crr_tick_value = current_tick.value_or(get_current_tick());
		total_paused_ticks += crr_tick_value - paused_tick;
		paused_tick = crr_tick_value;
	}
	const bool& Timer::is_paused() const { return paused; }
	int64_t Timer::make_point(const std::optional<Uint64>& current_tick) noexcept
	{
		if (paused) create_pause_snapshot(current_tick);

		const Uint64 crr_tick_value = current_tick.value_or(get_current_tick());
		const auto passed_time = crr_tick_value - start_tick - total_paused_ticks;
		const auto res = start_time + static_cast<int64_t>(passed_time);
		last_get_time = res;
		return res;
	}
	int64_t Timer::get_last_point() const noexcept
	{
		return last_get_time.value_or(start_time);
	}
	void Timer::pause(const std::optional<Uint64>& current_tick)
	{
		if (paused) return;
		const Uint64 crr_tick_value = current_tick.value_or(get_current_tick());
		paused_tick = crr_tick_value;
		paused = true;
	}
	void Timer::resume()
	{
		if (!paused) return;
		create_pause_snapshot();
		paused = false;
	}
	void Timer::reset(
		const bool auto_start,
		const std::optional<int64_t>& start_time,
		const std::optional<Uint64>& start_tick)
	{
		if (start_time.has_value()) this->start_time = start_time.value();
		this->start_tick = start_tick.value_or(get_current_tick());
		this->total_paused_ticks = 0;
		this->paused_tick = this->start_tick;
		this->paused = false;
		this->last_get_time = std::nullopt;
		if (!auto_start) pause(start_tick);
	}
	Timer::Timer(
		const int64_t& start_time,
		const bool auto_start,
		const std::optional<Uint64>& start_tick,
		std::function<Uint64()> get_current_tick_function)
		: get_current_tick(std::move(get_current_tick_function)),
		start_tick(start_tick.value_or(this->get_current_tick())),
		start_time(start_time)
	{
		if (!auto_start) pause(start_tick);
	}
}