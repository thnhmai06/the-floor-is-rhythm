#pragma once
#include <functional>
#include <optional>
#include <SDL3/SDL_timer.h>

namespace Structures::Events::Time
{
	struct Timer
	{
		std::function<Uint64()> get_current_tick;

	protected:
		Uint64 start_tick = 0;
		Uint64 total_paused_ticks = 0;
		Uint64 paused_tick = 0;
		bool paused = false;
		std::optional<int64_t> last_get_time;

		void create_pause_snapshot(const std::optional<Uint64>& current_tick = std::nullopt);

	public:
		int64_t start_time = 0;

		//! Chú ý:
		//! - Nếu có pause nhạc thì cũng cần pause lại timer
		//! - Khi seek nhạc, cần phải clear timer (autostart = true) và đặt start_time tại lúc seek

		[[nodiscard]] const bool& is_paused() const;
		[[nodiscard]] int64_t get_time(const std::optional<Uint64>& current_tick = std::nullopt) noexcept;
		[[nodiscard]] int64_t get_last_get_time() noexcept;
		void pause(const std::optional<Uint64>& current_tick = std::nullopt);
		void resume();
		void reset(
			bool auto_start = false,
			const std::optional<int64_t>& start_time = std::nullopt,
			const std::optional<Uint64>& start_tick = std::nullopt);

		explicit Timer(
			const int64_t& start_time = 0,
			bool auto_start = false,
			const std::optional<Uint64>& start_tick = std::nullopt, 
			std::function<Uint64()> get_current_tick_function = SDL_GetTicks);
	};
}