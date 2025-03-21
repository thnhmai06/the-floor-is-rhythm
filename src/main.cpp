#include "main.h" // Header
#include <SDL3/SDL.h>
#include "logging.h"
#include "render/update.h"
#include "expections/sdl.h"

constexpr char NAME[] = "The Floor is Rhythm";
constexpr bool DEBUG_MODE = true;

static void init(SDL_Window*& window, SDL_GPUDevice*& device, SDL_Renderer*& renderer)
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) THROW_CRITICAL(SDL_Exceptions::SDL::SDL_InitSDL_Failed());

	window = SDL_CreateWindow(NAME, 1366, 768, SDL_WINDOW_HIDDEN);
	if (!window) THROW_CRITICAL(SDL_Exceptions::SDL::SDL_CreateWindow_Failed());

	device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_DXBC, DEBUG_MODE, nullptr);
	if (!device) THROW_CRITICAL(SDL_Exceptions::SDL::SDL_CreateGPUDevice_Failed());

	if (!SDL_ClaimWindowForGPUDevice(device, window)) throw SDL_Exceptions::SDL::SDL_ClaimWindowForGPUDevice_Failed();
	SPDLOG_INFO("GPU Device Driver: {}", SDL_GetGPUDeviceDriver(device));

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) THROW_CRITICAL(SDL_Exceptions::SDL::SDL_CreateRenderer_Failed());
}

int32_t main(int32_t argc, char* argv[])
{
	Logging::init("root");

	try
	{
		SDL_Window* window = nullptr;
		SDL_GPUDevice* device = nullptr;
		SDL_Renderer* renderer = nullptr;
		init(window, device, renderer);

		SDL_ShowWindow(window);

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

				// work here  
				update_screen(device, window);
			}
		}

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		return EXIT_FAILURE;
	}
}