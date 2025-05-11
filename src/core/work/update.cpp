#include "core/work/update.h" // Header
#include "main.h"
#include "logging/logger.h"
#include "core/resources/event.h"
#include "core/resources/layer.h"
#include "core/resources/screen.h"

namespace Core::Work
{
	namespace Update
	{
		int64_t render()
		{
			const auto previous_render_time = Resources::Event::runtime.get_time();

			SDL_RenderClear(renderer);
			Resources::Layers::render_all();
			SDL_RenderPresent(renderer);

			return Resources::Event::runtime.get_time() - previous_render_time;
		}
		int64_t event()
		{
			const auto current_time = Resources::Event::runtime.get_time();

			Structures::Events::Event::Input::SdlEvents events;
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_QUIT)
				{
					is_running = false;
					break;
				}
				events.push_back(event);
			}
			Resources::Screens::gameplay_screen->logic.make_time_step(events);
			Resources::Event::action_buffer.execute(current_time);

			return Resources::Event::runtime.get_time() - current_time;
		}
		int64_t update()
		{
			const auto previous_update_time = Resources::Event::runtime.get_time();
			using namespace Update;

			if (const auto event_latency = event();
				event_latency > 20) SPDLOG_WARN("[Event] latency {}ms is very high!", event_latency);

			if (const auto render_latency = render();
				render_latency > 20) SPDLOG_WARN("[Render] latency {}ms is very high!", render_latency);

			return Resources::Event::runtime.get_time() - previous_update_time;
		}
	}
}
