#include "work/render.h"  //Header
#include <chrono>
#include <filesystem>
#include "work/core.h"
#include "main.h"
#include "logging/logger.h"
#include "structures/screen/gameplay.h"
#include "structures/events/event/input.h"
#include "work/audio.h"
#include "work/screen.h"
#include "work/render/layer.h"
#include "work/render/textures.h"

namespace Work::Render
{
	void init(SDL_Window* window)
	{
		Core::Init::renderer(window, renderer);
		Textures::init_all(renderer);
		Layers::init_all(renderer);
	}

	void clean()
	{
		Layers::clear_all();
		Core::CleanUp::renderer(renderer);
	}

	int32_t work()
	{
		const std::filesystem::path skin_path = "./assets";
		int32_t result = EXIT_SUCCESS;

		//try
		//{
		Textures::skin->load(skin_path, skin_path, true, false);
		Audio::Memory::Skin::effect.load(skin_path, skin_path, false);
		Screen::gameplay_screen = std::make_unique<Screen::Gameplay::GameplayScreen>
			(R"(D:\1511778 Camellia - Body F10ating in the Zero Gravity Space\Camellia - Body F10ating in the Zero Gravity Space (Orange_) [Nonsubmersible].tfd)", 
				1, true, true, true);

		SDL_Event event;
		while (is_running)
		{
			const auto previous_update_time = std::chrono::system_clock::now();
			const auto previous_render_time = std::chrono::system_clock::now();

			// render
			SDL_RenderClear(renderer);
			Layers::render_all();
			SDL_RenderPresent(renderer);

			// input
			const auto previous_input_time = std::chrono::system_clock::now();
			static Structures::Events::Event::Input::SdlEvents events;
			events.clear();
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_QUIT)
				{
					is_running = false;
					break;
				}
				events.push_back(event);
			}
			//const auto input_latency = Utilities::Time::get_duration<std::chrono::milliseconds>(previous_input_time);
			//SPDLOG_DEBUG("Input Latency: {}ms", input_latency);
			Screen::gameplay_screen->logic.make_time_step(events);

			//const auto render_latency = Utilities::Time::get_duration<std::chrono::milliseconds>(previous_render_time);
			//SPDLOG_DEBUG("Render Latency: {}ms", render_latency);

			const auto update_latency = Utilities::Time::get_duration<std::chrono::milliseconds>(previous_update_time);
			if (update_latency > 20) SPDLOG_WARN("Update Latency: {}ms is very high!", update_latency);
		}
		//} catch (...) {
		//	is_running = false;
		//	result = EXIT_FAILURE;
		//}

		return result;
	}
}