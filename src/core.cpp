#include "core.h" // Header
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "config.h"
#include "logger/exceptions.h"
#include "logger/logging.h"

//! Core
using namespace Core;
// ::Init
void Init::window(SDL_Window*& window)
{
	window = SDL_CreateWindow(
		GameConfig::General::NAME, 
		UserConfig::Video::width, 
		UserConfig::Video::height,
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
	SPDLOG_INFO("structures/render/er: {}", SDL_GetRendererName(renderer));

	SDL_SetRenderLogicalPresentation(
		renderer,	
		GameConfig::Video::LOGICAL_WIDTH, 
		GameConfig::Video::LOGICAL_HEIGHT,
		SDL_LOGICAL_PRESENTATION_STRETCH);
}

//:: CleanUp
void CleanUp::window(SDL_Window* window) { SDL_DestroyWindow(window); }
void CleanUp::renderer(SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); }