#include "thread/render.h" // Header
#include "render/texture.h"
#include "main.h"

int RenderThread::Worker(void* data)
{
	SDL_Renderer* renderer = static_cast<SDL_Renderer*>(data);
	TextureMemory memory(renderer);
	memory.load(R"(D:\PROGRAM\osu!\Songs\2287992 Camellia - Operation_ Zenithfall\Camellia_-_Operation_Zenithfall_2560x1440.jpg)", "bg");

	try
	{
		SDL_FRect rect = { 40, 40, 100, 100 };
		while (running) {
			SDL_RenderClear(renderer);
			memory.render("bg", { .dst_rect = nullptr });
			SDL_RenderPresent(renderer);
		}
	} catch (...) {
		running = false;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

SDL_Thread* RenderThread::start_thread(SDL_Renderer*& renderer) { return SDL_CreateThread(Worker, name, renderer); }