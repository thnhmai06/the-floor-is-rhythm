#include "main.h" // Header
#include "work/render.h"
#include "work/core.h"
#include "work/audio.h"
#include "work/convert/osu/beatmap.h"

int32_t main(int32_t argc, char* argv[])
{
	bool DEBUG_MODE = false;
	for (int32_t i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], "--debug") == 0)
			DEBUG_MODE = true;
	}

	Work::Core::Init::system(DEBUG_MODE);
	Work::Core::Init::window(window);
	Work::Audio::init();
	Work::Render::init(window);

	//Work::Convert::osu::convert_beatmap(R"(D:\PROGRAM\osu!\Songs\1511778 Camellia - Body F10ating in the Zero Gravity Space)", "D:\\");
	int32_t result = Work::Render::work();

	Work::Render::clean();
	Work::Audio::quit();
	Work::Core::CleanUp::window(window);
	Work::Core::CleanUp::system();

	return result;
}
