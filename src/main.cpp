#include <iostream>
#include <SDL3/SDL.h>  
#include <quill/LogMacros.h>  
#include "expections/sdl.h"
#include "logging.h"
#include "render/screen.h"

constexpr char NAME[] = "The Floor is Rhythm";
constexpr bool DEBUG_MODE = true;

static void init(SDL_Window*& window, SDL_GPUDevice*& device)
{
	if (!SDL_Init(SDL_INIT_VIDEO)) throw SDL_Exception::Critical::SDL_InitSDL_Failed();

	window = SDL_CreateWindow(NAME, 1366, 768, SDL_WINDOW_HIDDEN);
	if (!window) throw SDL_Exception::Critical::SDL_CreateWindow_Failed();

	device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_DXBC, DEBUG_MODE, nullptr);
	if (!device) throw SDL_Exception::Critical::SDL_CreateGPUDevice_Failed();

	if (!SDL_ClaimWindowForGPUDevice(device, window)) throw SDL_Exception::Critical::SDL_ClaimWindowForGPUDevice_Failed();
	LOG_INFO(LogSystem::logger, "GPU Device Driver: {}", SDL_GetGPUDeviceDriver(device));
}

int32_t main(int32_t argc, char* argv[])
{
	LogSystem::init();

	try
	{
		SDL_Window* window = nullptr;
		SDL_GPUDevice* device = nullptr;
		init(window, device);

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
				render_screen(device, window);
			}
		}

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		LOG_CRITICAL(LogSystem::logger, "{}", e.what());
		return EXIT_FAILURE;
	}
}