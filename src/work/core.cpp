#include "work/core.h" // Header
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "config.h"
#include "logging/exceptions.h"
#include "logging/logger.h"

//! Core
namespace Work::Core
{
	namespace Init
	{
		void system(const bool debug)
		{
			Logging::Logger::init("root", debug ? spdlog::level::debug : spdlog::level::info);
			if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
				THROW_CRITICAL(Logging::Exceptions::SDLExceptions::Video::SDL_Video_InitSDL_Failed());
		}
		void window(SDL_Window*& window)
		{
			window = SDL_CreateWindow(
				Config::GameConfig::General::NAME,
				Config::UserConfig::Video::width,
				Config::UserConfig::Video::height,
				SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_HIGH_PIXEL_DENSITY);
			if (!window)
				THROW_CRITICAL(Logging::Exceptions::SDLExceptions::Video::SDL_Video_CreateWindow_Failed());
		}
		void renderer(SDL_Window* window, SDL_Renderer*& renderer)
		{
			renderer = SDL_CreateRenderer(window, nullptr);
			if (!renderer)
				THROW_CRITICAL(Logging::Exceptions::SDLExceptions::Video::SDL_Video_CreateRenderer_Failed());

			SPDLOG_INFO("GPU Driver: {}", SDL_GetCurrentVideoDriver());
			SPDLOG_INFO("Renderer: {}", SDL_GetRendererName(renderer));

			SDL_SetRenderLogicalPresentation(
				renderer,
				Config::GameConfig::Video::LOGICAL_WIDTH,
				Config::GameConfig::Video::LOGICAL_HEIGHT,
				SDL_LOGICAL_PRESENTATION_STRETCH);
		}
	}

	namespace CleanUp
	{
		void system() { SDL_Quit(); }
		void window(SDL_Window* window) { SDL_DestroyWindow(window); }
		void renderer(SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); }
	}
}