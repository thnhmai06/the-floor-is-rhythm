#include "work/render.h" // Header
#include "core.h"
#include "main.h"
#include "format/skin.h"
#include "skin.h"
#include "parser/tfir/beatmap.h"

using namespace WorkingComponents::Render;

int32_t render(SDL_Window* window)
{
	const char* skin_path = "./assets";
	int32_t result = EXIT_SUCCESS;

	SDL_Renderer* renderer = nullptr;
	Core::Init::renderer(window, renderer);
	TextureMemory::init_all(renderer);
	Layers::init_all();

	try
	{
		auto current_direction = Template::Game::Direction::Direction::UP;
		load_skin(skin_path, *TextureMemory::skin);
		BeatmapFile beatmap(R"(D:\output.tfd)");
		Layers::playground->load_beatmap(beatmap.hit_objects, beatmap.calculated_difficulty, beatmap.timing_points);
		//Layers::playground->render_range.emplace_back(0, 200);

		Layers::cursor->load_cursor(&current_direction);
		current_direction = Template::Game::Direction::Direction::DOWN;
		Layers::cursor->components.direction->update_src();

		SDL_Event quit_event;
		while (running) {
			// quit event
			if (SDL_PollEvent(&quit_event) && quit_event.type == SDL_EVENT_QUIT) running = false;

			// render
			SDL_RenderClear(renderer);
			Layers::render_all();
			Layers::playground->camera.move_x(10);
			SDL_RenderPresent(renderer);
		}
	} catch (...) {
		running = false;
		result = EXIT_FAILURE;
	}
	Layers::reset_all(true);
	Core::CleanUp::renderer(renderer);

	return result;
}
