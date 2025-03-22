#include "main.h" // Header
#define STB_IMAGE_IMPLEMENTATION
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>
#include "logging.h"
#include "exceptions.h"
#include "thread/render.h"

constexpr char NAME[] = "The Floor is Rhythm";
constexpr bool DEBUG_MODE = true;

static void init(SDL_Window*& window, SDL_Renderer*& renderer)
{
	Logging::init("root", DEBUG_MODE ? spdlog::level::debug : spdlog::level::info);

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) THROW_CRITICAL(SDL_Exceptions::Init::SDL_InitSDL_Failed());
	window = SDL_CreateWindow(NAME, 1366, 768, SDL_WINDOW_HIDDEN);
	if (!window) THROW_CRITICAL(SDL_Exceptions::Init::SDL_CreateWindow_Failed());
	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) THROW_CRITICAL(SDL_Exceptions::Init::SDL_CreateRenderer_Failed());

	SPDLOG_INFO("GPU Driver: {}", SDL_GetCurrentVideoDriver());
	SPDLOG_INFO("Renderer: {}", SDL_GetRendererName(renderer));
}
static void quit(SDL_Window*& window, SDL_Renderer*& renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int32_t main(int32_t argc, char* argv[])
{
	int32_t res = EXIT_SUCCESS;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	init(window, renderer);
	SDL_ShowWindow(window);

	SDL_Thread* thread = RenderThread::start_thread(renderer);

	SDL_Event event;
	while (running) if (SDL_PollEvent(&event) && event.type == SDL_EVENT_QUIT) running = false;

	SDL_WaitThread(thread, &res);
	quit(window, renderer);
	return res;
}