#pragma once
#include <cstdint>
#include <unordered_map>

namespace Engine::Render
{
	constexpr uint8_t SDL_MAX_ALPHA = 255;
	struct Color { uint8_t r, g, b; }; // giống SDL_Color nhưng bỏ alpha

	enum class OriginType : std::uint8_t
	{
		TopLeft = 0, Centre = 1, CentreLeft = 2, TopRight = 3,
		BottomCentre = 4, TopCentre = 5, Custom = 6,
		CentreRight = 7, BottomLeft = 8, BottomRight = 9
	};

	namespace Object
	{
		enum class LoopType : bool
		{
			LoopForever = false,
			LoopOnce = true
		};

		using Alphabet = std::unordered_map<char, std::string>;
	}
}
