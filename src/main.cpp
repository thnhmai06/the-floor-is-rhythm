#include <iostream>
#include <convert/osu/beatmap.h>
#include <SDL2/SDL.h>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

void wait_key_pressed()
{
	SDL_Event e;
	while (true) {
		if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT)) return;
		SDL_Delay(100);
	}
}

int main(int argc, char *argv[])
{
	/*CONVERT_OSU_BEATMAP_H::convert(
		"D:\\PROGRAM\\osu!\\Songs\\2287992 Camellia - Operation_ Zenithfall\\Camellia - Operation Zenithfall (Mir) [Final Mission].osu", 
		"D:\\output.tfd"
	);*/
	// init
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	// set bg
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// draw rect
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	constexpr int SIZE = 100;
	constexpr int POS_X = 200;
	constexpr int POS_Y = 200;
	SDL_Rect rect = { POS_X, POS_Y, SIZE, SIZE };
	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderPresent(renderer);

	wait_key_pressed();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}