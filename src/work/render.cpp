#include "work/render.h"  //Header
#include <chrono>
#include <filesystem>
#include "work/core.h"
#include "main.h"
#include "logging/logger.h"
#include "structures/screens/gameplay/playing.h"
#include "structures/action/event/input.h"
#include "work/skin.h"

using namespace Work::Render;

int32_t Work::Render::render(SDL_Window* window)
{
	const std::filesystem::path skin_path = "./assets";
	int32_t result = EXIT_SUCCESS;

	SDL_Renderer* renderer = nullptr;
	Core::Init::renderer(window, renderer);
	Textures::init_all(renderer);
	Layers::init_all();

	//try
	//{
		Skin::load_image(skin_path, *Textures::skin, skin_path);
		Screens::playing_screen = std::make_unique<Screens::Gameplay::PlayingScreen>
	("D:\\2258410 Kagetora - UNPR3C3D3NT3D TRAV3L3R\\Kagetora. - UNPR3C3D3NT3D TRAV3L3R (Ducky-) [F1N4L D3ST1N4T10N].osu.tfd");

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
			static Structures::Action::Event::Input::EventList events;
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
			const auto input_latency = Utilities::Time::get_duration<std::chrono::milliseconds>(previous_input_time);
			//SPDLOG_DEBUG("Input Latency: {}ms", input_latency);
			Screens::playing_screen->logic_core.make_time_step(events, static_cast<float>(input_latency));

			const auto render_latency = Utilities::Time::get_duration<std::chrono::milliseconds>(previous_render_time);
			//SPDLOG_DEBUG("Render Latency: {}ms", render_latency);

			const auto update_latency = Utilities::Time::get_duration<std::chrono::milliseconds>(previous_update_time);
			//SPDLOG_DEBUG("Update Latency: {}ms", update_latency);
		}
	//} catch (...) {
	//	is_running = false;
	//	result = EXIT_FAILURE;
	//}
	Layers::reset_all(true);
	Core::CleanUp::renderer(renderer);

	return result;
}
