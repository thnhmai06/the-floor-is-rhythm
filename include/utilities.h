#pragma once
#include <chrono>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <SDL3_mixer/SDL_mixer.h>

namespace Utilities
{
	namespace Math
	{
		inline bool is_bit_enabled(const std::int32_t value, const std::int32_t bitmask) { return (value & bitmask) != 0; }
		inline bool is_equal_float(const float& variable, const float value, const float epsilon = 0.01f) { return abs(variable - value) <= epsilon; }
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
	namespace Time
	{
		inline std::string get_current_time(const char* format = "%Y-%m-%d_%H-%M-%S")
		{
			const auto now = std::chrono::system_clock::now();
			const auto time_t_now = std::chrono::system_clock::to_time_t(now);

			std::stringstream ss;
			ss << std::put_time(std::localtime(&time_t_now), format);
			return ss.str();
		}
	}
}
