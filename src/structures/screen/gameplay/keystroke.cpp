#include "structures/screen/gameplay/keystroke.h" // Header

namespace Structures::Screen::Gameplay::KeyStroke
{
	namespace Logic
	{
		//! KeyCounter
		void KeyCounter::update(const KeyboardEvents& events)
		{
			recently_pressed_num = 0;
			for (const auto& event : events)
			{
				if (event.scancode != target) continue;
				is_hold = event.repeat; // nếu thả ra, sẽ có event KeyUp để cập nhật
				if (event.down && !event.repeat)
				{
					recently_pressed_num++;
					count++;
				}
				else recently_pressed_num = 0; // tránh trường hợp spam press và hold
			}
		}
		void KeyCounter::reset()
		{
			count = recently_pressed_num = 0;
			is_hold = false;
		}
		KeyCounter::KeyCounter(const SDL_Scancode& target) : target(target)
		{
		}

		//! KeyStroke
		uint16_t Keystroke::get_recently_pressed_left() const
		{
			return l1.recently_pressed_num + l2.recently_pressed_num;
		}
		uint16_t Keystroke::get_recently_pressed_right() const
		{
			return r1.recently_pressed_num + r2.recently_pressed_num;
		}
		void Keystroke::update(const KeyboardEvents& events)
		{
			l1.update(events);  l2.update(events);
			r1.update(events);  r2.update(events);
		}
		void Keystroke::reset()
		{
			l1.reset(); l2.reset();
			r1.reset(); r2.reset();
		}
	}
}