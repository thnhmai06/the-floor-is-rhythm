#include "coremgr.h" // Header
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "rule/config.h"
#include "exceptions.h"
#include "logging.h"

//! Video
void Init::window(SDL_Window*& window)
{
	window = SDL_CreateWindow(GameConfig::General::NAME, 1366, 768,
		SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	if (!window)
		THROW_CRITICAL(SDLExceptions::Video::SDL_Video_CreateWindow_Failed());
}
void Init::renderer(SDL_Window* window, SDL_Renderer*& renderer)
{
	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer)
		THROW_CRITICAL(SDLExceptions::Video::SDL_Video_CreateRenderer_Failed());

	SPDLOG_INFO("GPU Driver: {}", SDL_GetCurrentVideoDriver());
	SPDLOG_INFO("Renderer: {}", SDL_GetRendererName(renderer));

	// UserConfig
	SDL_SetRenderLogicalPresentation(renderer, GameConfig::Video::LOGICAL_WIDTH, GameConfig::Video::LOGICAL_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH);
}

//! Clean up
void CleanUp::window(SDL_Window* window) { SDL_DestroyWindow(window); }
void CleanUp::renderer(SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); }