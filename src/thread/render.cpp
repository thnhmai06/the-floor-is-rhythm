#include "thread/render.h" // Header
#include "render/texture.h"
#include "main.h"

int RenderThread::Worker(void* data)
{
	SDL_Renderer* renderer = static_cast<SDL_Renderer*>(data);
	try
	{
		TextureMemory memory(renderer);
		memory.load(R"(D:\\Development\\Code\\cpp\\Repositories\\the-floor-is-rhythm\\assets\\floor.png)", "floor");
		memory.load(R"(D:\\PROGRAM\\osu!\\Songs\\2287992 Camellia - Operation_ Zenithfall\\Camellia_-_Operation_Zenithfall_2560x1440.jpg)", "bg");
		
		SDL_FRect rect = { 40, 40, 100, 100 };
		while (running) {
			SDL_RenderClear(renderer);
			memory.render("bg", nullptr);
			memory.render("floor", &rect);
			rect.x += 0.05f;
			SDL_RenderPresent(renderer);
		}
	} catch (...) {
		running = false;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

SDL_Thread* RenderThread::start_thread(SDL_Renderer*& renderer) { return SDL_CreateThread(Worker, name, renderer); }