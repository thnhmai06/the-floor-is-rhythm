#include "core/manager.h" // Header
#include "config.h"
#include "main.h"
#include "logging/logger.h"

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
				THROW_CRITICAL(Logging::Exceptions::Engine::System::SDL_System_InitSDL_Failed());
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
				THROW_CRITICAL(Logging::Exceptions::Engine::System::SDL_System_CreateWindow_Failed());
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
				THROW_CRITICAL(Logging::Exceptions::Engine::System::SDL_System_CreateRenderer_Failed());

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
			mixer = std::make_unique<Engine::Audio::Mixer>(
				MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_WAVPACK, spec,
				::Config::user_config->audio.volume.master, ::Config::user_config->audio.volume.music, 
				::Config::user_config->audio.volume.effect, MAX_CHANNELS);
		}
	}
	namespace Skin
	{
		void load()
		{
			texture = std::make_shared<Engine::Render::Texture::Memory>(renderer);
			effect = std::make_shared<Engine::Audio::Memory::EffectMemory>();

			texture->load(::Config::Game::General::Path::SKIN, ::Config::Game::General::Path::SKIN, true, false);
			effect->load(::Config::Game::General::Path::SKIN, ::Config::Game::General::Path::SKIN, false);
		}
	}

	void init()
	{
		System::init();
		Config::init();
		Window::init();
		Renderer::init();
		Mixer::init();
		Skin::load();
	}
	void quit()
	{
		Renderer::quit();
		Window::quit();
		Config::quit();
		System::quit();
	}
}