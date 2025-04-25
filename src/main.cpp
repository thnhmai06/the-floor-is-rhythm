#include "main.h" // Header
#include <SDL3/SDL_init.h>
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "work/render.h"
#include "work/core.h"

#include "structures/game/beatmap.h"
#include "work/audio.h"
#include "work/convert/osu/beatmap.h"
#include "work/convert/osu/mapset.h"

constexpr bool DEBUG_MODE = true;

int32_t main(int32_t argc, char* argv[])
{
	int32_t result = EXIT_SUCCESS;
	SDL_Window* window = nullptr;
	Work::Core::Init::system(DEBUG_MODE);
	Work::Core::Init::window(window);
	Work::Audio::init();

	// Render
	Work::Convert::osu::convert_beatmap("D:\\PROGRAM\\osu!\\Songs\\2258410 Kagetora - UNPR3C3D3NT3D TRAV3L3R", "D:\\");
	result = Work::Render::render(window);

	Work::Audio::quit();
	Work::Core::CleanUp::window(window);
	Work::Core::CleanUp::system();
}
