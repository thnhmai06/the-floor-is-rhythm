#pragma once
#include "game/hitobject.h"

namespace Skin_Filename
{
	namespace Cursor
	{
		const std::string NAME = "cursor";
		const std::string TAIL = "cursortail";
		const std::unordered_map<Direction, std::string> DIRECTION = {
			{Direction::RIGHT, "right"},
			{Direction::UP, "up"},
			{Direction::DOWN, "down"},
			{Direction::LEFT, "left"}
		};
	}
	namespace HitObject
	{
		const std::string FLOOR = "floor";
		namespace Slider
		{
			const std::string BODY = "sliderb";
			const std::string BEGIN = "sliderbegin";
			const std::string LINE = "sliderline";
			const std::string POINT = "sliderpoint";
			const std::string CURVE = "slidercurve";
			const std::string END = "sliderend";
		}
	}
}
