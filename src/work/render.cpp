#include "work/render.h"  //Header
#include "work/core.h"
#include "main.h"

#include <chrono>
#include "logging/logger.h"
#include "structures/screens/gameplay/playing.h"
#include "format/skin.h"
#include "work/skin.h"

using namespace Work::Render;

int32_t Work::Render::render(SDL_Window* window)
{
	const auto skin_path = "./assets";
	int32_t result = EXIT_SUCCESS;

	SDL_Renderer* renderer = nullptr;
	Core::Init::renderer(window, renderer);
	Textures::init_all(renderer);
	Layers::init_all();

	try
	{
		Skin::load_skin(skin_path, *Textures::skin);

		SDL_Event quit_event;
		while (is_running) 
		{
			const auto previous_render = std::chrono::system_clock::now();

			// quit event
			if (SDL_PollEvent(&quit_event) && quit_event.type == SDL_EVENT_QUIT) is_running = false;

			// render
			SDL_RenderClear(renderer);
			Layers::render_all();
			Layers::playground->camera.move_x(10);
			SDL_RenderPresent(renderer);

			const auto after_render = std::chrono::system_clock::now();
			const auto latency = std::chrono::duration_cast<std::chrono::milliseconds>(after_render - previous_render).count();
			SPDLOG_DEBUG("Render Latency: {}ms", latency);
		}
	} catch (...) {
		is_running = false;
		result = EXIT_FAILURE;
	}
	Layers::reset_all(true);
	Core::CleanUp::renderer(renderer);

	return result;
}
