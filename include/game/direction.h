#pragma once
#include <cstdint>

namespace Direction
{
	enum class Direction : uint8_t
	{
		RIGHT = 0,
		UP = 1,
		DOWN = 2,
		LEFT = 3
	};
	enum class Rotation : uint8_t
	{
		NO_ROTATE = 0,
		ROTATE_90 = 1,
		ROTATE_180 = 2,
		ROTATE_270 = 3
	};
	constexpr Direction operator+(const Direction& direction, const Rotation& rotation)
	{
		return static_cast<Direction>((static_cast<uint8_t>(direction) + static_cast<uint8_t>(rotation)) % 4);
	}
	constexpr Direction operator+=(Direction& direction, const Rotation& rotation)
	{
		direction = direction + rotation;
		return direction;
	}
}
