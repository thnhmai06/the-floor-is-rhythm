#include "work/render.h" // Header
#include "coremgr.h"
#include "main.h"
#include "rule/skin.h"

int32_t render(SDL_Window* window)
{
	int32_t result = EXIT_SUCCESS;


	//// test
	//Current_Layers::background->memory.load(
	//	R"(D:\PROGRAM\osu!\Songs\2287992 Camellia - Operation_ Zenithfall\Camellia_-_Operation_Zenithfall_2560x1440.jpg)",
	//	"camellia");
	//Current_Layers::playground->memory.load(
	//	R"(D:\Development\Code\cpp\Repositories\the-floor-is-rhythm\assets\floor.png)", "floor");
	//Current_Layers::background->render_list.emplace_back("camellia", TextureRenderConfig());
	//const auto bg = &Current_Layers::background->render_list.back();
	//Current_Layers::playground->render_list.emplace_back("floor", TextureRenderConfig());
	//const auto obj = &Current_Layers::playground->render_list.back();

	//bg->second.alpha = 100;
	//auto obj_dstrect = SDL_FRect{ 100, 100, 60, 40 };
	//obj->second.dst_rect = &obj_dstrect;
	
	SDL_Renderer* renderer = Init::renderer(window);
	Current_Layers::init_all(renderer);

	/*Current_Layers::background->memory.load(R"(D:\Development\Code\cpp\Repositories\the-floor-is-rhythm\assets\floor.png)", Skin_Filename::HitObject::FLOOR);
	SDL_FRect size = { 100, 100, , ImmutableConfig::HitObject::SIZE_HEIGHT };
	TextureRenderConfig config(&size);
	const auto obj = RenderObject::RenderObject(&Skin_Filename::HitObject::FLOOR, config);
	Current_Layers::background->render_list.emplace_back(obj);*/

	try
	{
		SDL_Event quit_event;
		while (running) {
			// quit event
			if (SDL_PollEvent(&quit_event) && quit_event.type == SDL_EVENT_QUIT) running = false;

			// render
			SDL_RenderClear(renderer);
			Current_Layers::render_all();
			SDL_RenderPresent(renderer);
		}
	} catch (...) {
		running = false;
		result = EXIT_FAILURE;
	}
	Current_Layers::clear_all(true);
	CleanUp::renderer(renderer);

	return result;
}
