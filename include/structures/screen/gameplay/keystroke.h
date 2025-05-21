#pragma once
#include "config.h"
#include "engine/events/condition.h"
#include "engine/events/event.h"

namespace Structures::Screen::Gameplay::KeyStroke
{
	namespace Logic
	{
		using Engine::Events::Condition::External::Condition;

		struct KeyCounter
		{
		protected:
			unsigned long count = 0;
			uint8_t recently_pressed_num = 0;
			bool is_hold = false;

		public:
			SDL_Scancode target;

			void make_virtual_event(bool is_pressed = true, bool is_hold = false);
			void update(const Engine::Events::Event::External::Buffer::TypeViewer& events);
			[[nodiscard]] const uint8_t& get_recently_pressed_num() const;
			[[nodiscard]] const unsigned long& get_count() const;
			[[nodiscard]] const bool& get_is_hold() const;
			void reset(bool only_recently_pressed_num = false);

			explicit KeyCounter(const SDL_Scancode& target);
		};

		struct Keystroke
		{
			const Engine::Events::Event::External::Buffer* buffer;
			std::weak_ptr<const Engine::Events::Timing::Timer> timer;

			KeyCounter l1{ ::Config::user_config->gameplay.binding.l1 };
			KeyCounter l2{ ::Config::user_config->gameplay.binding.l2 };
			KeyCounter r1{ ::Config::user_config->gameplay.binding.r1 };
			KeyCounter r2{ ::Config::user_config->gameplay.binding.r2 };

			void reset(bool only_recently_pressed_num = false);
			void update();

			[[nodiscard]] uint16_t get_recently_pressed_left() const;
			[[nodiscard]] uint16_t get_recently_pressed_right() const;

			explicit Keystroke(const Engine::Events::Event::External::Buffer& buffer, std::weak_ptr<const Engine::Events::Timing::Timer> timer);
		};
	}
}
