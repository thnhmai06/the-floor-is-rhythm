#include "work/render.h" // Header
#include "coremgr.h"
#include "main.h"
#include "rule/skin.h"

int32_t render(SDL_Window* window)
{
	int32_t result = EXIT_SUCCESS;
	SDL_Renderer* renderer = nullptr;
	Init::renderer(window, renderer);

	//// test
	//WorkingLayers::background->memory.load(
	//	R"(D:\PROGRAM\osu!\Songs\2287992 Camellia - Operation_ Zenithfall\Camellia_-_Operation_Zenithfall_2560x1440.jpg)",
	//	"camellia");
	//WorkingLayers::playground->memory.load(
	//	R"(D:\Development\Code\cpp\Repositories\the-floor-is-rhythm\assets\floor.png)", "floor");
	//WorkingLayers::background->render_buffer.emplace_back("camellia", TextureConfig());
	//const auto bg = &WorkingLayers::background->render_buffer.back();
	//WorkingLayers::playground->render_buffer.emplace_back("floor", TextureConfig());
	//const auto obj = &WorkingLayers::playground->render_buffer.back();

	//bg->second.alpha = 100;
	//auto obj_dstrect = SDL_FRect{ 100, 100, 60, 40 };
	//obj->second.dst_rect = &obj_dstrect;

	WorkingLayers::init_all(renderer);
	/*WorkingLayers::background->memory.load(R"(D:\Development\Code\cpp\Repositories\the-floor-is-rhythm\assets\floor.png)", SkinFormat::HitObjects::FLOOR);
	SDL_FRect size = { 100, 100, , GameConfig::HitObjects::SIZE_HEIGHT };
	TextureConfig config(&size);
	const auto obj = RenderObjects::RenderObjects(&SkinFormat::HitObjects::FLOOR, config);
	WorkingLayers::background->render_buffer.emplace_back(obj);*/

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
