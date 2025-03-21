#include "main.h" // Header
#include <SDL3/SDL.h>
#include "logging.h"
#include "exceptions/sdl.h"
#include "render/texture.h"
#include "render/update.h"

constexpr char NAME[] = "The Floor is Rhythm";
constexpr bool DEBUG_MODE = true;

static void init(SDL_Renderer*& renderer)
{
	Logging::init("root");
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) THROW_CRITICAL(SDL_Exceptions::Init::SDL_InitSDL_Failed());

	window = SDL_CreateWindow(NAME, 1366, 768, SDL_WINDOW_HIDDEN);
	if (!window) THROW_CRITICAL(SDL_Exceptions::Init::SDL_CreateWindow_Failed());
	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) THROW_CRITICAL(SDL_Exceptions::Init::SDL_CreateRenderer_Failed());

	SPDLOG_INFO("GPU Driver: {}", SDL_GetCurrentVideoDriver());
	SPDLOG_INFO("Renderer: {}", SDL_GetRendererName(renderer));
}
static void quit(SDL_Renderer*& renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int RenderThread(void* data)
{
	SDL_Renderer* renderer = static_cast<SDL_Renderer*>(data);
	TextureMemory memory(renderer);
	auto it = memory.load(R"(D:\Development\Code\cpp\Repositories\the-floor-is-rhythm\assets\floor.png)", "floor");
	SDL_FRect rect = { 40, 40, 100, 100 };

	while (running) {
		SDL_RenderClear(renderer);
		memory.render("floor", &rect);
		rect.x += 0.1f;
		SDL_RenderPresent(renderer);
	}
	return 0;
}

int32_t main(int32_t argc, char* argv[])
{
	int8_t res = EXIT_SUCCESS;
	SDL_Renderer* renderer = nullptr;
	init(renderer);

	SDL_Thread* thread = SDL_CreateThread(RenderThread, "RenderThread", renderer);

	try
	{
		SDL_ShowWindow(window);
		SDL_Event event;
		while (running) if (SDL_PollEvent(&event) && event.type == SDL_EVENT_QUIT) running = false;
	} catch (...) {
		res = EXIT_FAILURE;
	}

	SDL_WaitThread(thread, nullptr);
	quit(renderer);
	return res;
}