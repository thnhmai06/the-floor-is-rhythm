#pragma once
#include "game/object.h"

namespace Skin_Filename
{
	namespace Cursor
	{
		constexpr std::string_view NAME = "cursor";
		constexpr std::string_view TAIL = "cursortail";
		const std::unordered_map<Direction, std::string_view> DIRECTION = {
			{Direction::RIGHT, "right"},
			{Direction::UP, "up"},
			{Direction::DOWN, "down"},
			{Direction::LEFT, "left"}
		};
	}
	namespace HitObject
	{
		constexpr std::string_view FLOOR = "floor";
		namespace Slider
		{
			constexpr std::string_view BODY = "sliderb";
			constexpr std::string_view BEGIN = "sliderbegin";
			constexpr std::string_view LINE = "sliderline";
			constexpr std::string_view POINT = "sliderpoint";
			constexpr std::string_view CURVE = "slidercurve";
			constexpr std::string_view END = "sliderend";
		}
	}
}
