#include "core/work.h"  //Header
#include <filesystem>
#include "main.h"
#include "logging/logger.h"
#include "core/resources/screen.h"
#include "core/resources/event.h"
#include "core/work/update.h"

namespace Core::Work
{
	int32_t work()
	{
		int32_t result = EXIT_SUCCESS;

		//try
		//{
		Resources::Screens::gameplay_screen = std::make_unique<Structures::Screen::Gameplay::GameplayScreen>
			(R"(D:\1511778 Camellia - Body F10ating in the Zero Gravity Space\Camellia - Body F10ating in the Zero Gravity Space (Orange_) [Nonsubmersible].tfd)", 
				1, true, true, true);

		while (is_running)
		{
			if (const auto update_latency = Update::update();
				update_latency > 20) SPDLOG_WARN("[Update] latency {}ms is very high!", update_latency);
		}

		Resources::Screens::gameplay_screen.reset();

		//} catch (...) {
		//	is_running = false;
		//	result = EXIT_FAILURE;
		//}
		return result;
	}
}
