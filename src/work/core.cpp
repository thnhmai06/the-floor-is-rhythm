#include "work/core.h" // Header
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "config.h"

//! Core
namespace Work::Core
{
	namespace Init
	{
		void config()
		{
			const fs::path path = SDL_GetBasePath() / fs::path("config.ini");

			if (fs::exists(path))
			{
				Config::user_config = std::make_unique<Structures::Config::UserConfig>(path);
				return;
			}
			SPDLOG_WARN("Configuration file not found. A default config file will be generated.");
			Config::user_config = std::make_unique<Structures::Config::UserConfig>();
			Config::user_config->write(path);
		}
		void system(const bool debug)
		{
			Logging::Logger::init("root", debug ? spdlog::level::debug : spdlog::level::info);
			if (debug) LOG_INFO("Game is running on Debug mode!");
			if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
				THROW_CRITICAL(Logging::Exceptions::SDLExceptions::Video::SDL_Video_InitSDL_Failed());
		}
		void window(SDL_Window*& window)
		{
			if (Config::user_config->graphic.fullscreen)
			{
				window = SDL_CreateWindow(
					Config::Game::General::NAME,
					Config::user_config->graphic.window_size.x,
					Config::user_config->graphic.window_size.y,
					SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_FULLSCREEN);
			}
			else
			{
				window = SDL_CreateWindow(
					Config::Game::General::NAME,
					Config::user_config->graphic.window_size.x,
					Config::user_config->graphic.window_size.y,
					SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_HIGH_PIXEL_DENSITY);
			}
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

			/*SDL_SetRenderLogicalPresentation(
				renderer,
				Config::Game::Render::LOGICAL_WIDTH,
				Config::Game::Render::LOGICAL_HEIGHT,
				SDL_LOGICAL_PRESENTATION_STRETCH);*/
		}
	}

	namespace CleanUp
	{
		void config()
		{
			const fs::path path = SDL_GetBasePath() / fs::path("config.ini");
			Config::user_config->write(path);
			Config::user_config.reset();
		}
		void system() { SDL_Quit(); }
		void window(SDL_Window* window) { SDL_DestroyWindow(window); }
		void renderer(SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); }
	}
}