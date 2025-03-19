#pragma once
#include <cstdint>

namespace Utilities
{
	namespace Math
	{
		inline bool is_bit_enabled(const std::int32_t value, const std::int32_t bitmask) { return (value & bitmask) != 0; }
		inline bool is_equal_float(const float& variable, const float value, const float epsilon = 0.01) { return abs(variable - value) <= epsilon; }
	}
}