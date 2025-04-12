#include "structures/action.h" // Header
#include "config.h"

inline size_t std::hash<SDL_Scancode>::operator()(const SDL_Scancode& key) const noexcept
{
	return static_cast<size_t>(key);
}

namespace Structures::Action
{
	namespace Key
	{
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
		void KeyObserver::update(KeyQueue& queue)
		{
			update();

			while (!queue.empty())
			{
				if (const auto& [is_press_event, key] = queue.front(); is_press_event)
				{
					if (hold_only_keys.contains(key)) continue;
					insert_or_assign(key, KeyStatus{ .is_pressed = true, .is_hold = true });
					hold_only_keys.insert(key);
					need_update_to_hold_only.push(key);
				}
				else
				{
					insert_or_assign(key, KeyStatus{ .is_pressed = false, .is_hold = false });
					hold_only_keys.erase(key);
				}
				queue.pop();
			}
		}

		// ::PlayingKeyQueue
		PlayingKeyQueue::PlayingKeyQueue()
		{
			filter.emplace_back(Config::UserConfig::KeyBinding::Click::is_click);
			filter.emplace_back(Config::UserConfig::KeyBinding::Direction::is_direction);
		}
	}
	namespace Time
	{
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
}