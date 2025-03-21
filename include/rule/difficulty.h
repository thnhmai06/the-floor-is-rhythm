#pragma once
#include <cstdint>

namespace Difficulty
{
	namespace AR
	{
		constexpr float PREEMPT_AR5 = 1200;
		constexpr float FADE_IN_AR5 = 800;
	}

	namespace OD
	{
		namespace Base
		{
			constexpr int32_t PERFECT = 80; // 300
			constexpr int32_t GOOD = 140; // 100
			constexpr int32_t BAD = 200; // 50
		}

		namespace Multiply
		{
			constexpr int32_t PERFECT = 6; // 300
			constexpr int32_t GOOD = 8; // 100
			constexpr int32_t BAD = 10; // 50
		}
	}
}
