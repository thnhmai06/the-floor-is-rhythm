#pragma once
#include "config.h"
#include "structures/events/event/input.h"

namespace Structures::Screen::Gameplay::KeyStroke
{
	namespace Logic
	{
		using Events::Event::Input::KeyboardEvents;

		struct KeyCounter
		{
		protected:
			SDL_Scancode target;
			unsigned long count = 0;
			uint8_t recently_pressed_num = 0;
			bool is_hold = false;

		public:
			void make_virtual_event(bool count = true, bool is_hold = false);
			void update(const KeyboardEvents& events);
			[[nodiscard]] const uint8_t& get_recently_pressed_num() const;
			[[nodiscard]] const unsigned long& get_count() const;
			[[nodiscard]] const bool& get_is_hold() const;
			void reset(bool only_recently_pressed_num = false);

			explicit KeyCounter(const SDL_Scancode& target);
		};

		struct Keystroke
		{
			KeyCounter l1{ ::Config::user_config->gameplay.binding.l1 };
			KeyCounter l2{ ::Config::user_config->gameplay.binding.l2 };
			KeyCounter r1{ ::Config::user_config->gameplay.binding.r1 };
			KeyCounter r2{ ::Config::user_config->gameplay.binding.r2 };

			void reset(bool only_recently_pressed_num = false);
			void update(const KeyboardEvents& events);

			[[nodiscard]] uint16_t get_recently_pressed_left() const;
			[[nodiscard]] uint16_t get_recently_pressed_right() const;
		};
	}
}
