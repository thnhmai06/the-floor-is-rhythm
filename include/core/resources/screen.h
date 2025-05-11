#pragma once
#include "structures/screen/gameplay.h"

namespace Core::Resources
{
	namespace Screens
	{
		using namespace Structures::Screen;

		inline std::unique_ptr<Gameplay::GameplayScreen> gameplay_screen;
	}
}