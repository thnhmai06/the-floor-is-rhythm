#include "structures/screen/gameplay/keystroke.h" // Header

namespace Structures::Screen::Gameplay::KeyStroke
{
	namespace Logic
	{
		//! KeyCounter
		void KeyCounter::make_virtual_event(const bool is_pressed, const bool is_hold)
		{
			if (is_pressed)
			{
				this->count++;
				++recently_pressed_num;
			}
			else reset(true);
			this->is_hold = is_hold;
		}
		const uint8_t& KeyCounter::get_recently_pressed_num() const { return recently_pressed_num; }
		const unsigned long& KeyCounter::get_count() const { return count; }
		const bool& KeyCounter::get_is_hold() const { return is_hold; }
		void KeyCounter::update(const KeyboardEvents& events)
		{
			reset(true);
			for (const auto& event : events)
			{
				if (event.scancode != target) continue;
				is_hold = event.repeat; // nếu thả ra, sẽ có event KeyUp để cập nhật
				if (event.down && !event.repeat)
				{
					recently_pressed_num++;
					count++;
				}
				else reset(true); // tránh trường hợp spam press và hold
			}
		}
		void KeyCounter::reset(const bool only_recently_pressed_num)
		{
			if (only_recently_pressed_num)
			{
				recently_pressed_num = 0;
				return;
			}
			count = recently_pressed_num = 0;
			is_hold = false;
		}
		KeyCounter::KeyCounter(const SDL_Scancode& target) : target(target)
		{
		}

		//! KeyStroke
		uint16_t Keystroke::get_recently_pressed_left() const
		{
			return l1.get_recently_pressed_num() + l2.get_recently_pressed_num();
		}
		uint16_t Keystroke::get_recently_pressed_right() const
		{
			return r1.get_recently_pressed_num() + r2.get_recently_pressed_num();
		}
		void Keystroke::update(const KeyboardEvents& events)
		{
			l1.update(events);  l2.update(events);
			r1.update(events);  r2.update(events);
		}
		void Keystroke::reset(const bool only_recently_pressed_num)
		{
			l1.reset(only_recently_pressed_num); l2.reset(only_recently_pressed_num);
			r1.reset(only_recently_pressed_num); r2.reset(only_recently_pressed_num);
		}
	}
}