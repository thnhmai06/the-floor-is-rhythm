#include "coremgr.h" // Header
#include "config.h"
#include "exceptions.h"
#include "logging.h"

//! Init
SDL_Window* Init::window()
{
	const auto window = SDL_CreateWindow(Immutable::General::NAME, 1366, 768,
		SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	if (!window)
		THROW_CRITICAL(SDL_Exceptions::Init::SDL_CreateWindow_Failed());
	return window;
}
SDL_Renderer* Init::renderer(SDL_Window* window)
{
	const auto renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer)
		THROW_CRITICAL(SDL_Exceptions::Init::SDL_CreateRenderer_Failed());
	SPDLOG_INFO("GPU Driver: {}", SDL_GetCurrentVideoDriver());
	SPDLOG_INFO("Renderer: {}", SDL_GetRendererName(renderer));

	// Config
	SDL_SetRenderLogicalPresentation(renderer, Immutable::Video::LOGICAL_WIDTH, Immutable::Video::LOGICAL_HEIGHT, SDL_LOGICAL_PRESENTATION_STRETCH);

	return renderer;
}

//! Clean up
void CleanUp::window(SDL_Window* window) { SDL_DestroyWindow(window); }
void CleanUp::renderer(SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); }
