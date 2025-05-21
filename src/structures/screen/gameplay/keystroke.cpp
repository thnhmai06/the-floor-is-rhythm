#include "structures/screen/gameplay/keystroke.h" // Header
#include <ranges>
#include "structures/events/condition/external/pressed.h"

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
		void KeyCounter::update(const Engine::Events::Event::External::Buffer::TypeViewer& events)
		{
			reset(true);
			for (const auto& event : events | std::views::values)
			{
				const auto& key_event = event.key;

				if (key_event.scancode != target) continue;
				is_hold = key_event.repeat; // nếu thả ra, sẽ có event KeyUp để cập nhật
				if (key_event.down && !key_event.repeat)
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
		void Keystroke::update()
		{
			if (const auto locked_timer = Utilities::Pointer::check_weak(timer))
			{
				const auto current_time = locked_timer->get_last_point();
				const auto condition = std::make_shared<Events::Condition::External::External::KeyPressCondition>(
					std::unordered_set{l1.target, l2.target, r1.target, r2.target}, current_time);

				auto raw = buffer->search(condition);
				raw[SDL_EVENT_KEY_DOWN].merge(raw[SDL_EVENT_KEY_UP]);
				const auto& events = raw[SDL_EVENT_KEY_DOWN];

				l1.update(events);  l2.update(events);
				r1.update(events);  r2.update(events);
			}
		}
		void Keystroke::reset(const bool only_recently_pressed_num)
		{
			l1.reset(only_recently_pressed_num); l2.reset(only_recently_pressed_num);
			r1.reset(only_recently_pressed_num); r2.reset(only_recently_pressed_num);
		}
		Keystroke::Keystroke(const Engine::Events::Event::External::Buffer& buffer, std::weak_ptr<const Engine::Events::Timing::Timer> timer)
			: buffer(&buffer), timer(std::move(timer))
		{
		}
	}
}
