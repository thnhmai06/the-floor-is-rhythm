#pragma once
#include <cstdint>
#include <cmath>
#include <SDL3_mixer/SDL_mixer.h>

namespace Utilities
{
	namespace Math
	{
		inline bool is_bit_enabled(const std::int32_t value, const std::int32_t bitmask) { return (value & bitmask) != 0; }
		inline bool is_equal_float(const float& variable, const float value, const float epsilon = 0.01) { return abs(variable - value) <= epsilon; }
	}

	namespace Audio
	{
		inline int32_t get_volume(const int32_t v)
		{
			if (v < 0) return -1;
			return (MIX_MAX_VOLUME * v) / 100;
		}
		inline int32_t get_real_volume(const int32_t v)
		{
			return (v * 100) / MIX_MAX_VOLUME;
		}
	}
}
