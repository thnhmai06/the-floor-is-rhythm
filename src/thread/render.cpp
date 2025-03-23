#include "thread/render.h" // Header
#include "coremgr.h"
#include "exceptions.h"
#include "main.h"

int32_t RenderThread::worker(void* window)
{
	int32_t result = EXIT_SUCCESS;
	SDL_Renderer* renderer = Init::renderer(static_cast<SDL_Window*>(window));

	try
	{
		WorkingLayer::init(renderer);
		while (running) {
			SDL_RenderClear(renderer);
			WorkingLayer::render();
			SDL_RenderPresent(renderer);
		}
	} catch (...) {
		running = false;
		result = EXIT_FAILURE;
	}

	WorkingLayer::free();
	CleanUp::renderer(renderer);
	return result;
}

SDL_Thread* RenderThread::start_thread(void* window) { return SDL_CreateThread(worker, NAME, window); }