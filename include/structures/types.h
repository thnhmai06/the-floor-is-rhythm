#pragma once
#include <concepts>
#include <SDL3_mixer/SDL_mixer.h>
#include <unordered_map>
#include <string>

namespace Structures::Types
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
		enum class OriginType : uint8_t
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
		namespace HitObject
		{
			enum class HitObjectType : bool
			{
				Floor = 0,
				Slider = 1
			};
		}
	}
	namespace Format
	{
		namespace Skin
		{
			using Alphabet = std::unordered_map<char, std::string>;
		}
	}
}