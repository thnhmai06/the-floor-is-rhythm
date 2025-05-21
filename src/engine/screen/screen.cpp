#include "engine/screen/screen.h" // Header
#include "utilities.h"

namespace Engine::Screen
{
	//! Screen
	const bool& Screen::get_active() const { return active; }
	const bool& Screen::get_enabled() const { return enabled; }
	const bool& Screen::get_finished() const { return finished; }
	void Screen::on_activate() { active = true; }
	void Screen::on_deactivate() { active = false; }

	//! Stack
	Uint64 Stack::update()
	{
		const auto previous_tick = SDL_GetTicks();

		// Event
		external_events.scan();

		// Normal
		while (!normal.empty() && normal.top().empty())
			normal.top().pop_back();
		if (!normal.empty())
		{
			auto& top = normal.top();
			auto ptr = top.begin();
			while (ptr != top.end())
			{
				const auto& screen = *ptr;

				if (screen->get_enabled())
					screen->update(*this);
				if (screen->get_finished())
					ptr = top.erase(ptr);
				else ++ptr;
			}
		}

		// Sticky
		auto sticky_ptr = sticky.begin();
		while (sticky_ptr != sticky.end())
		{
			const auto& screen = *sticky_ptr;

			if (screen->get_enabled())
				screen->update(*this);
			if (screen->get_finished())
				sticky_ptr = sticky.erase(sticky_ptr);
			else ++sticky_ptr;
		}
		return SDL_GetTicks() - previous_tick;
	}
}
