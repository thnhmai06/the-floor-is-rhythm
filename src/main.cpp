#pragma comment(linker, "/ENTRY:mainCRTStartup")
extern "C"
{
	using DWORD = unsigned long;

	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#include "main.h" // Header
#include "core/task/convert/osu/beatmap.h"
#include "core/manager.h"
#include "structures/screen/gameplay.h"

int32_t main(const int32_t argc, char* argv[])
{
	for (int32_t i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], "--debug") == 0) DEBUG = true;
	}

	constexpr auto beatmap_path = R"(D:\PROGRAM\osu!\Songs\2287992 Camellia - Operation_ Zenithfall)";
	constexpr auto mapset_path = R"(D:\Development\Code\cpp\Repositories\the-floor-is-rhythm\build\out\x64\Debug\beatmap\2287992 Camellia - Operation_ Zenithfall\Camellia - Operation Zenithfall (Mir) [Final Mission].tfd)";

	Core::Manager::init();
	Engine::Screen::Stack screen_stack;
	//Core::Task::Convert::Osu::convert_beatmap(beatmap_path);
	screen_stack.normal.push({
		std::make_shared<Structures::Screen::Gameplay::GameplayScreen>(
			screen_stack.external_events, mapset_path,
			Core::Manager::Skin::texture, Core::Manager::Skin::effect,
			1, true, true, true)
		});

	// Game loop
	while (is_running)
	{
		static bool need_print_latency = false;
		SDL_RenderClear(renderer);
		if (const auto update_latency = screen_stack.update();
			update_latency > 20)
		{
			SPDLOG_WARN("Update latency: {}ms", update_latency);
			need_print_latency = true;
		}
		else
		{
			if (need_print_latency)
				LOG_DEBUG("Update latency: {}ms", update_latency);
			need_print_latency = false;
		}
		SDL_RenderPresent(renderer);
	}

	Core::Manager::quit();
	return EXIT_SUCCESS;
}