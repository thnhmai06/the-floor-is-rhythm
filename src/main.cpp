#pragma comment(linker, "/ENTRY:mainCRTStartup")
extern "C"
{
	using DWORD = unsigned long;

	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#include "main.h" // Header
#include "core/manager.h"
#include "core/work.h"

int32_t main(const int32_t argc, char* argv[])
{
	for (int32_t i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], "--debug") == 0) DEBUG = true;
	}

	Core::Manager::init();
	const int32_t result = Core::Work::run();
	Core::Manager::quit();
	
	return result;
}