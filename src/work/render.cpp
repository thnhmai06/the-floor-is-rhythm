#include "work/render.h" // Header
#include "coremgr.h"
#include "main.h"
#include "rule/skin.h"

int32_t render(SDL_Window* window)
{
	int32_t result = EXIT_SUCCESS;
	SDL_Renderer* renderer = nullptr;
	Init::renderer(window, renderer);

	WorkingLayers::init_all(renderer);

	try
	{
		SDL_Event quit_event;
		while (running) {
			// quit event
			if (SDL_PollEvent(&quit_event) && quit_event.type == SDL_EVENT_QUIT) running = false;

			// render
			SDL_RenderClear(renderer);
			WorkingLayers::render_all();
			SDL_RenderPresent(renderer);
		}
	} catch (...) {
		running = false;
		result = EXIT_FAILURE;
	}
	WorkingLayers::clear_all(true);
	CleanUp::renderer(renderer);

	return result;
}
