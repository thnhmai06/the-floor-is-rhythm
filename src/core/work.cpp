#include "core/work.h"  //Header
#include <filesystem>
#include "main.h"
#include "logging/logger.h"
#include "core/resources/screen.h"
#include "core/work/update.h"
#include "core/work/convert/osu/beatmap.h"
#include "core/work/convert/osu/mapset.h"

namespace Core::Work
{
	int32_t run()
	{
		int32_t result = EXIT_SUCCESS;

		//try
		//{
		//Convert::Osu::convert_beatmap(R"(D:\2048051 Sad Keyboard Guy & Gardens - Waking Shadows feat. eili.osz)");
		Resources::Screens::gameplay_screen = std::make_unique<Structures::Screen::Gameplay::GameplayScreen>
			(R"(D:\Development\Code\cpp\Repositories\the-floor-is-rhythm\build\out\x64\Debug\beatmap\1511778 Camellia - Body F10ating in the Zero Gravity Space\Camellia - Body F10ating in the Zero Gravity Space (Orange_) [Nonsubmersible].tfd)", 
				1, true, true, true);

		while (is_running)
		{
			static bool need_print_latency = false;
			if (const auto update_latency = Update::update();
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
		}

		Resources::Screens::gameplay_screen.reset();

		//} catch (...) {
		//	is_running = false;
		//	result = EXIT_FAILURE;
		//}
		return result;
	}
}
