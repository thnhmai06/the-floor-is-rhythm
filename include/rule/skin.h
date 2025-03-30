#pragma once
#include "template.h"
#include "game/hitobject.h"

namespace SkinFormat
{
	using ConfigValue = const std::string;

	namespace Cursor
	{
		ConfigValue BODY = "cursor";
		ConfigValue TAIL = "cursortail";
		inline std::unordered_map<Template::Game::Direction::Direction, ConfigValue> DIRECTION = {
			{Template::Game::Direction::Direction::RIGHT, "right"},
			{Template::Game::Direction::Direction::UP, "up"},
			{Template::Game::Direction::Direction::DOWN, "down"},
			{Template::Game::Direction::Direction::LEFT, "left"}
		};
	}
	namespace HitObject
	{
		// ReSharper disable once CppUnnamedNamespaceInHeaderFile
		namespace
		{
			ConfigValue SUFFIX_FLOOR = "floor";
			namespace Suffic_Slider {
				ConfigValue SUFFIX_FOCUS = "sliderfocus";
				ConfigValue SUFFIX_BEGIN = "sliderbegin";
				ConfigValue SUFFIX_LINE = "sliderline";
				ConfigValue SUFFIX_POINT = "sliderpoint";
				ConfigValue SUFFIX_CURVE = "slidercurve";
				ConfigValue SUFFIX_END = "sliderend";
			}
		}
		namespace Left
		{
			static ConfigValue DIRECTION = "left_";

			ConfigValue FLOOR = DIRECTION + SUFFIX_FLOOR;
			namespace Slider
			{
				ConfigValue FOCUS = DIRECTION + Suffic_Slider::SUFFIX_FOCUS;
				ConfigValue BEGIN = DIRECTION + Suffic_Slider::SUFFIX_BEGIN;
				ConfigValue LINE = DIRECTION + Suffic_Slider::SUFFIX_LINE;
				ConfigValue POINT = DIRECTION + Suffic_Slider::SUFFIX_POINT;
				ConfigValue CURVE = DIRECTION + Suffic_Slider::SUFFIX_CURVE;
				ConfigValue END = DIRECTION + Suffic_Slider::SUFFIX_END;
			}
		}
		namespace Right
		{
			static ConfigValue DIRECTION = "right_";

			ConfigValue FLOOR = DIRECTION + SUFFIX_FLOOR;
			namespace Slider
			{
				ConfigValue FOCUS = DIRECTION + Suffic_Slider::SUFFIX_FOCUS;
				ConfigValue BEGIN = DIRECTION + Suffic_Slider::SUFFIX_BEGIN;
				ConfigValue LINE = DIRECTION + Suffic_Slider::SUFFIX_LINE;
				ConfigValue POINT = DIRECTION + Suffic_Slider::SUFFIX_POINT;
				ConfigValue CURVE = DIRECTION + Suffic_Slider::SUFFIX_CURVE;
				ConfigValue END = DIRECTION + Suffic_Slider::SUFFIX_END;
			}
		}
		namespace Up
		{
			static ConfigValue DIRECTION = "up_";

			ConfigValue FLOOR = DIRECTION + SUFFIX_FLOOR;
			namespace Slider
			{
				ConfigValue FOCUS = DIRECTION + Suffic_Slider::SUFFIX_FOCUS;
				ConfigValue BEGIN = DIRECTION + Suffic_Slider::SUFFIX_BEGIN;
				ConfigValue LINE = DIRECTION + Suffic_Slider::SUFFIX_LINE;
				ConfigValue POINT = DIRECTION + Suffic_Slider::SUFFIX_POINT;
				ConfigValue CURVE = DIRECTION + Suffic_Slider::SUFFIX_CURVE;
				ConfigValue END = DIRECTION + Suffic_Slider::SUFFIX_END;
			}
		}
		namespace Down
		{
			static ConfigValue DIRECTION = "down_";

			ConfigValue FLOOR = DIRECTION + SUFFIX_FLOOR;
			namespace Slider
			{
				ConfigValue FOCUS = DIRECTION + Suffic_Slider::SUFFIX_FOCUS;
				ConfigValue BEGIN = DIRECTION + Suffic_Slider::SUFFIX_BEGIN;
				ConfigValue LINE = DIRECTION + Suffic_Slider::SUFFIX_LINE;
				ConfigValue POINT = DIRECTION + Suffic_Slider::SUFFIX_POINT;
				ConfigValue CURVE = DIRECTION + Suffic_Slider::SUFFIX_CURVE;
				ConfigValue END = DIRECTION + Suffic_Slider::SUFFIX_END;
			}
		}
	}
}
