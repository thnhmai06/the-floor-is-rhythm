#include "main.h" // Header
#include <SDL3/SDL_init.h>
#include "logger/logging.h"
#include "logger/exceptions.h"
#include "work/render.h"
#include "work/core.h"

#include "parser/tfir/beatmap.h"
#include "parser/osu/beatmap.h"

constexpr bool DEBUG_MODE = true;

int32_t main(int32_t argc, char* argv[])
{
	int32_t result = EXIT_SUCCESS;
	Logging::init("root", DEBUG_MODE ? spdlog::level::debug : spdlog::level::info);
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
		THROW_CRITICAL(SDLExceptions::Video::SDL_Video_InitSDL_Failed());

	// Window & Renderer
	SDL_Window* window = nullptr;
	Work::Core::Init::window(window);

	// Render
	result = Work::Render::render(window);
	/*convert_beatmap(
		R"(D:\PROGRAM\osu!\Songs\2287992 Camellia - Operation_ Zenithfall\Camellia - Operation Zenithfall (Mir) [Final Mission].osu)",
		R"(D:\output.tfd)");
	Mapset beatmap(R"(D:\output.tfd)");*/

	Work::Core::CleanUp::window(window);
	return result;
}