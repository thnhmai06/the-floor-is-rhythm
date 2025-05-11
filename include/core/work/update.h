#pragma once
#include <cstdint>

namespace Core::Work 
{
	namespace Update
	{
		int64_t render();
		int64_t event();
		int64_t update();
	}
}
