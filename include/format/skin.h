// ReSharper disable StringLiteralTypo
#pragma once
#include <array>
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
			inline std::unordered_map<Structures::Types::Game::Direction::Direction, const std::string> direction_skin =
			{
				{Structures::Types::Game::Direction::Direction::RIGHT, "right/direction"},
				{Structures::Types::Game::Direction::Direction::UP, "up/direction"},
				{Structures::Types::Game::Direction::Direction::DOWN, "down/direction"},
				{Structures::Types::Game::Direction::Direction::LEFT, "left/direction"}
			};
		}
		namespace HitObject
		{
			enum class HitObjectType : uint8_t
			{
				FLOOR, SLIDER_FOCUS, SLIDER_LINE, SLIDER_POINT
			};
			constexpr uint8_t HitObjectType_NUM = 4;

			struct HitObjectsInDirection : std::array<std::string, HitObjectType_NUM>
			{
			protected:
				using VALUE = std::string;
				using BASE = std::array<VALUE, HitObjectType_NUM>;

			private:
				VALUE direction_folder;
				static std::string get_direction_folder(const Structures::Types::Game::Direction::Direction& direction);

			public:
				reference operator[] (const HitObjectType& type);
				const_reference operator[] (const HitObjectType& type) const;
				explicit HitObjectsInDirection(const Structures::Types::Game::Direction::Direction& direction);
			};
			struct HitObjectSkins : std::array<HitObjectsInDirection, Structures::Types::Game::Direction::NUM_DIRECTIONS>
			{
			protected:
				using VALUE = HitObjectsInDirection;
				using BASE = std::array<VALUE, Structures::Types::Game::Direction::NUM_DIRECTIONS>;

			public:
				reference operator[] (const Structures::Types::Game::Direction::Direction& direction);
				const_reference operator[] (const Structures::Types::Game::Direction::Direction& direction) const;
				explicit HitObjectSkins();
			};

			inline const HitObjectSkins hit_objects_skin;
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
	}

	namespace Sound
	{
		inline const std::unordered_set<std::string_view> SUPPORT_EXTENSIONS = { ".wav", ".mp3", ".ogg" };
	}

	struct ImageNamespace : std::unordered_set<std::string> { ImageNamespace(); } inline const image_namespace;
	struct FolderNamespace : std::unordered_set<std::string> { FolderNamespace(); } inline const folder_namespace;
}