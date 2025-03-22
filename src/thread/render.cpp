#include "thread/render.h" // Header
#include "render/texture.h"
#include "main.h"

int RenderThread::Worker(void* data)
{
	SDL_Renderer* renderer = static_cast<SDL_Renderer*>(data);
	try
	{
		SDL_FRect rect = { 40, 40, 100, 100 };
		while (running) {
			SDL_RenderClear(renderer);

			SDL_RenderPresent(renderer);
		}
	} catch (...) {
		running = false;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

SDL_Thread* RenderThread::start_thread(SDL_Renderer*& renderer) { return SDL_CreateThread(Worker, name, renderer); }