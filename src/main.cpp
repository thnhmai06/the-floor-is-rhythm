#include "main.h" // Header
#include <SDL3/SDL_init.h>
#include "logging.h"
#include "exceptions.h"
#include "work/render.h"
#include "coremgr.h"

#include "file/tfir/tfir_beatmap.h"
#include "file/osu/osu_beatmap.h"

constexpr bool DEBUG_MODE = true;

int32_t main(int32_t argc, char* argv[])
{
	int32_t result = EXIT_SUCCESS;
	Logging::init("root", DEBUG_MODE ? spdlog::level::debug : spdlog::level::info);
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
		THROW_CRITICAL(SDL_Exceptions::Init::SDL_InitSDL_Failed());

	// Window
	SDL_Window* window = Init::window();

	// Render
	/*result = render(window);*/
	convert_beatmap(
		R"(D:\PROGRAM\osu!\Songs\2287992 Camellia - Operation_ Zenithfall\Camellia - Operation Zenithfall (Mir) [Final Mission].osu)",
		R"(D:\output.tfd)");
	BeatmapFile beatmap{ R"(D:\output.tfd)" };

	CleanUp::window(window);
	return result;
}