#include "work/render.h" // Header
#include "coremgr.h"
#include "main.h"

int32_t render(SDL_Window* window)
{
	int32_t result = EXIT_SUCCESS;

	SDL_Renderer* renderer = Init::renderer(window);
	Current_Layers::init_all(renderer);

	// test
	Current_Layers::background->memory.load(
		R"(D:\PROGRAM\osu!\Songs\2287992 Camellia - Operation_ Zenithfall\Camellia_-_Operation_Zenithfall_2560x1440.jpg)",
		"camellia");
	Current_Layers::playground->memory.load(
		R"(D:\Development\Code\cpp\Repositories\the-floor-is-rhythm\assets\floor.png)", "floor");
	Current_Layers::background->objects.emplace_back("camellia", TextureRenderConfig());
	const auto bg = &Current_Layers::background->objects.back();
	Current_Layers::playground->objects.emplace_back("floor", TextureRenderConfig());
	const auto obj = &Current_Layers::playground->objects.back();

	bg->second.alpha = 100;
	auto obj_dstrect = SDL_FRect{ 100, 100, 60, 40 };
	obj->second.dst_rect = &obj_dstrect;
	
	try
	{
		SDL_Event quit_event;
		while (running) {
			// quit event
			if (SDL_PollEvent(&quit_event) && quit_event.type == SDL_EVENT_QUIT) running = false;

			// render
			SDL_RenderClear(renderer);
			Current_Layers::render_all();
			obj_dstrect.x += 0.05f;
			SDL_RenderPresent(renderer);
		}
	} catch (...) {
		running = false;
		result = EXIT_FAILURE;
	}
	Current_Layers::free_all();
	CleanUp::renderer(renderer);

	return result;
}