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
			SDL_Scancode target;
			unsigned long count = 0;
			uint8_t recently_pressed_num = 0;
			bool is_hold = false;

			void update(const KeyboardEvents& events);
			void reset();

			explicit KeyCounter(const SDL_Scancode& target);
		};

		struct Keystroke
		{
			KeyCounter l1{ Config::UserConfig::KeyBinding::l1 };
			KeyCounter l2{ Config::UserConfig::KeyBinding::l2 };
			KeyCounter r1{ Config::UserConfig::KeyBinding::r1 };
			KeyCounter r2{ Config::UserConfig::KeyBinding::r2 };

			void reset();
			void update(const KeyboardEvents& events);

			[[nodiscard]] uint16_t get_recently_pressed_left() const;
			[[nodiscard]] uint16_t get_recently_pressed_right() const;
		};
	}
}
