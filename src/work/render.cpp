#include "work/render.h" // Header
#include "coremgr.h"
#include "main.h"
#include "format/skin.h"
#include "skin.h"
#include "file/tfir/beatmap.h"

int32_t render(SDL_Window* window)
{
	int32_t result = EXIT_SUCCESS;
	SDL_Renderer* renderer = nullptr;
	Init::renderer(window, renderer);

	WorkingLayers::init_all(renderer);
	const char* skin_path = "./assets";

	try
	{
		load_skin(skin_path, WorkingLayers::playground->memory);
		BeatmapFile beatmap(R"(D:\output.tfd)");
		WorkingLayers::playground->run_beatmap(beatmap.hit_objects, beatmap.calculated_difficulty, beatmap.timing_points);
		WorkingLayers::playground->render_range = {0, 0};

		SDL_Event quit_event;
		while (running) {
			// quit event
			if (SDL_PollEvent(&quit_event) && quit_event.type == SDL_EVENT_QUIT) running = false;

			// render
			SDL_RenderClear(renderer);
			WorkingLayers::render_all();
			WorkingLayers::playground->camera.move_x(0.1f);
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
