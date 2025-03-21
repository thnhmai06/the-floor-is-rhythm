#include "main.h" // Header
#include <SDL3/SDL.h>
#include "logging.h"
#include "expections/sdl.h"
#include "render/update.h"
#include "render/texture.h"

constexpr char NAME[] = "The Floor is Rhythm";
constexpr bool DEBUG_MODE = true;

static void init(SDL_Window*& window, SDL_Renderer*& renderer)
{
	Logging::init("root");

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) THROW_CRITICAL(SDL_Exceptions::Init::SDL_InitSDL_Failed());

	window = SDL_CreateWindow(NAME, 1366, 768, SDL_WINDOW_HIDDEN);
	if (!window) THROW_CRITICAL(SDL_Exceptions::Init::SDL_CreateWindow_Failed());

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) THROW_CRITICAL(SDL_Exceptions::Init::SDL_CreateRenderer_Failed());
}

int32_t main(int32_t argc, char* argv[])
{
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	init(window, renderer);
	SDL_ShowWindow(window);

	Texture test(renderer, R"(D:\Development\Code\cpp\Repositories\the-floor-is-rhythm\assets\floor.png)", 100, 100);

	bool is_running = true;
	SDL_Event event;
	// main loop  
	while (is_running)
	{
		// event loop  
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				is_running = false;
				break;
			default: break;
			}
			SDL_RenderClear(renderer);
			// work here
			test.render(100.0f, 100.0f); // render texture
			SDL_RenderPresent(renderer);
		}
	}

	return EXIT_SUCCESS;
}