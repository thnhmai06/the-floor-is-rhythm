#pragma once
#include <string_view>
#include "game/object.h"

namespace Skin_Filename
{
	namespace Cursor
	{
		constexpr std::string_view NAME = "cursor";
		constexpr std::string_view TAIL = "cursortail";
		const std::unordered_map<Direction, std::string_view> DIRECTION
		= {
			{Direction::RIGHT, "c_right"},
			{Direction::UP, "c_up"},
			{Direction::DOWN, "c_down"},
			{Direction::LEFT, "c_left"}
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
		const std::unordered_map<Direction, std::string_view> DIRECTION
		= {
			{Direction::RIGHT, "d_right"},
			{Direction::UP, "d_up"},
			{Direction::DOWN, "d_down"},
			{Direction::LEFT, "d_left"}
		};
	}
}
