#include "core/manager.h" // Header
#include "main.h"
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "core/resources/textures.h"
#include "core/resources/audio.h"
#include "core/resources/layer.h"
#include "core/resources/screen.h"
#include "core/resources/event.h"

//! Manager
namespace Core::Manager
{
	namespace System
	{
		void init()
		{
			Logging::Logger::init("root", DEBUG ? spdlog::level::debug : spdlog::level::info);
			if (DEBUG) LOG_INFO("Game is running on Debug mode!");

			if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
				THROW_CRITICAL(Logging::Exceptions::SDLExceptions::System::SDL_System_InitSDL_Failed());
		}
		void quit()
		{
			SDL_Quit();
			spdlog::shutdown();
		}
	}
	namespace Config
	{
		void init()
		{
			const fs::path path = SDL_GetBasePath() / fs::path(::Config::Game::General::Path::CONFIG);

			if (fs::exists(path))
			{
				::Config::user_config = std::make_unique<Structures::Config::UserConfig>(path);
				return;
			}
			SPDLOG_WARN("Configuration file not found. A default config file will be generated.");
			::Config::user_config = std::make_unique<Structures::Config::UserConfig>();
			::Config::user_config->write(path);
		}
		void quit()
		{
			const fs::path path = SDL_GetBasePath() / fs::path(::Config::Game::General::Path::CONFIG);
			::Config::user_config->write(path);
			::Config::user_config.reset();
		}
	}
	namespace Window
	{
		void init()
		{
			auto flags = SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_HIGH_PIXEL_DENSITY;
			if (::Config::user_config->graphic.fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

			window = SDL_CreateWindow(
				::Config::Game::General::NAME,
				::Config::user_config->graphic.window_size.x,
				::Config::user_config->graphic.window_size.y,
				flags);

			if (!window)
				THROW_CRITICAL(Logging::Exceptions::SDLExceptions::System::SDL_System_CreateWindow_Failed());
		}
		void quit()
		{
			SDL_DestroyWindow(window);
		}
	}
	namespace Renderer
	{
		void init()
		{
			renderer = SDL_CreateRenderer(window, nullptr);
			if (!renderer)
				THROW_CRITICAL(Logging::Exceptions::SDLExceptions::System::SDL_System_CreateRenderer_Failed());

			SPDLOG_INFO("GPU Driver: {}", SDL_GetCurrentVideoDriver());
			SPDLOG_INFO("Renderer: {}", SDL_GetRendererName(renderer));
		}
		void quit()
		{
			SDL_DestroyRenderer(renderer);
		}
	}
	namespace Mixer
	{
		void init()
		{
			using namespace ::Config::Game::Audio;

			SDL_AudioSpec spec{ AUDIO_FORMAT, CHANNELS_STEREO, SAMPLE_FREQUENCY };
			mixer = std::make_unique<Structures::Audio::Mixer>(spec, ::Config::user_config->audio.volume, MAX_CHANNELS);
		}
		void quit()
		{
			if (mixer)
			{
				Structures::Audio::Mixer::quit();
				mixer.reset();
			}
		}
	}
	namespace Resources
	{
		using namespace Core::Resources;

		static void load()
		{
			const fs::path skin_path = SDL_GetBasePath() / fs::path(::Config::Game::General::Path::SKIN);

			Textures::skin->load(skin_path, skin_path, true, false);
			Audio::Skin::effect.load(skin_path, skin_path, false);
		}
		static void free()
		{
			Textures::skin->free_all();
			Audio::Skin::effect.free_all();

			Textures::skin.reset();
		}

		void init()
		{
			Textures::init_all(renderer);
			Layers::init_all(renderer);
			load();
		}
		void quit()
		{
			free();
			Layers::clear_all();
			Textures::clear_all();
		}
	}

	void init()
	{
		System::init();
		Resources::Event::runtime.resume();
		Config::init();
		Window::init();
		Renderer::init();
		Mixer::init();
		Resources::init();
	}
	void quit()
	{
		Resources::init();
		Mixer::quit();
		Renderer::quit();
		Window::quit();
		Config::quit();
		Resources::Event::runtime.pause();
		System::quit();
	}
}