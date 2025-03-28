#pragma once
#include "game/hitobject.h"

namespace SkinFormat
{
	using ConfigValue = const std::string;

	namespace Cursor
	{
		ConfigValue NAME = "cursor";
		ConfigValue TAIL = "cursortail";
		inline std::unordered_map<Direction, ConfigValue> DIRECTION = {
			{Direction::RIGHT, "right"},
			{Direction::UP, "up"},
			{Direction::DOWN, "down"},
			{Direction::LEFT, "left"}
		};
	}
	namespace HitObject
	{
		ConfigValue FLOOR = "floor";
		namespace Slider
		{
			ConfigValue BODY = "sliderb";
			ConfigValue BEGIN = "sliderbegin";
			ConfigValue LINE = "sliderline";
			ConfigValue POINT = "sliderpoint";
			ConfigValue CURVE = "slidercurve";
			ConfigValue END = "sliderend";
		}
	}
}
