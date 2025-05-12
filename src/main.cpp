#include "main.h" // Header
#include "core/manager.h"
#include "core/work.h"
#include "core/work/convert/osu/beatmap.h"

static int32_t run()
{
	using namespace Core::Work;

	//Convert::Osu::convert_beatmap(R"(D:\2258410 Kagetora. - UNPR3C3D3NT3D TRAV3L3R.osz)");
	int32_t result = work();

	return result;
}

int32_t main(const int32_t argc, char* argv[])
{
	for (int32_t i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], "--debug") == 0) DEBUG = true;
	}

	Core::Manager::init();
	const int32_t result = run();
	Core::Manager::quit();

	return result;
}