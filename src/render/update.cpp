#include "render/update.h" // Header

void update_screen(SDL_Renderer* renderer)
{
	SDL_RenderClear(renderer);
	// work
	SDL_RenderPresent(renderer);
}
