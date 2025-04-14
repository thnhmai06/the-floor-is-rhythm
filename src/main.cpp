#include "main.h" // Header
#include <SDL3/SDL_init.h>
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "work/render.h"
#include "work/core.h"

#include "structures/game/beatmap/beatmap.h"
#include "work/convert/osu/beatmap.h"

constexpr bool DEBUG_MODE = true;

int32_t main(int32_t argc, char* argv[])
{
	int32_t result = EXIT_SUCCESS;
	Logging::Logger::init("root", DEBUG_MODE ? spdlog::level::debug : spdlog::level::info);
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
		THROW_CRITICAL(Logging::Exceptions::SDLExceptions::Video::SDL_Video_InitSDL_Failed());

	// Window & Renderer
	SDL_Window* window = nullptr;
	Work::Core::Init::window(window);

	// Render
	Work::Convert::osu::convert_beatmap(
		R"(D:\PROGRAM\osu!\Songs\2287992 Camellia - Operation_ Zenithfall\Camellia - Operation Zenithfall (Mir) [Final Mission].osu)",
		R"(D:\output.tfd)");
	Structures::Game::Beatmap::Beatmap beatmap(R"(D:\output.tfd)");
	result = Work::Render::render(window);

	Work::Core::CleanUp::window(window);
	return result;
}
