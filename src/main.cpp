#pragma comment(linker, "/ENTRY:mainCRTStartup") // TODO: Hỗ trợ hàm main cái Window app
extern "C"
{
	using DWORD = unsigned long;

	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#include "main.h" // Header
#include "core/manager.h"
#include "core/work.h"
#include "core/work/convert/osu/beatmap.h"

static int32_t run()
{
	using namespace Core::Work;

	//Convert::Osu::convert_beatmap(R"(D:\PROGRAM\osu!\Songs\546820 YUC'e - Future Candy)");
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