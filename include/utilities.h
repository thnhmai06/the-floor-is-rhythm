#pragma once

#include <cstdint>

namespace Utilities
{
	inline bool is_bit_enabled(const std::int32_t value, const std::int32_t bitmask) { return (value & bitmask) != 0; }
}


