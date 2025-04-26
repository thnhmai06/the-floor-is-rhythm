// ReSharper disable StringLiteralTypo
#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "structures/types.h"

namespace Format::Skin
{
	namespace Image
	{
		inline const std::unordered_set<std::string_view> SUPPORT_EXTENSIONS = { ".png", ".jpg" };

		namespace Cursor
		{
			inline const std::string body = "cursor";
			inline const std::string trail = "cursortrail";
			inline const std::string focus = "cursorfocus";
		}
		namespace HitObject
		{
			inline const std::string floor = "floor";
			inline const std::string slider_line = "sliderline";
			inline const std::string slider_point = "sliderpoint";
			inline const std::string slider_end = "sliderend";
			inline const std::string slider_reverse = "sliderreverse";
		}
		namespace HealthBar
		{
			inline const std::string background = "healthbar-bg";
			inline const std::string colour = "healthbar-colour";
		}
		namespace Score
		{
			using Structures::Types::Format::Skin::Alphabet;

			static inline const std::string prefix = "score-";
			inline Alphabet alphabet =
			{
				{'0', prefix + '0'},
				{'1', prefix + '1'},
				{'2', prefix + '2'},
				{'3', prefix + '3'},
				{'4', prefix + '4'},
				{'5', prefix + '5'},
				{'6', prefix + '6'},
				{'7', prefix + '7'},
				{'8', prefix + '8'},
				{'9', prefix + '9'},
				{'.', prefix + "dot"},
				{',', prefix + "comma"},
				{'x', prefix + 'x'},
				{'%', prefix + "percent"}
			};
		}

		struct Namespace
		{
			std::unordered_set<std::string> data;

			Namespace();
		} const namespace_;
	}

	namespace Sound
	{
		inline const std::unordered_set<std::string_view> SUPPORT_EXTENSIONS = { ".wav", ".mp3", ".ogg" };
	}

	struct FolderNamespace
	{
		std::unordered_set<std::string> data;

		FolderNamespace();
	} inline const folder_namespace;
}