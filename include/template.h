#pragma once
#include <concepts>
#include <SDL3_mixer/SDL_mixer.h>

namespace Template
{
	namespace Audio
	{
		using Music = Mix_Music*;
		using Effect = Mix_Chunk*;

		template <typename T>
		concept AudioPtrType = std::same_as<T, Music> || std::same_as<T, Effect>;
	}
	namespace Render
	{
		enum class RenderOriginType : uint8_t
		{
			TOP_LEFT,
			CENTRE,
			BOTTOM_LEFT,
			BOTTOM_RIGHT,
			TOP_RIGHT,
			TOP_CENTRE,
			BOTTOM_CENTRE,
			CENTRE_LEFT,
			CENTRE_RIGHT
		};
	}
	namespace Game
	{
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
		namespace HitObject
		{
			enum class HitObjectType : bool
			{
				FLOOR = 0,
				SLIDER = 1
			};
		}
	}
}