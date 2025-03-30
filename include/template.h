#pragma once
#include <concepts>
#include <SDL3_mixer/SDL_mixer.h>
#include "render/object.h"

namespace Template
{
	namespace Audio
	{
		using Music = Mix_Music*;
		using Effect = Mix_Chunk*;

		template <typename T>
		concept AudioPtrType = std::same_as<T, Music> || std::same_as<T, Effect>;
	}

	namespace Video
	{
		namespace TextureOrigin
		{
			inline constexpr RenderConfig::RenderOriginPoint TOP_LEFT = { 0, 0 };
			constexpr RenderConfig::RenderOriginPoint CENTRE(const float& w, const float& h) { return { w / 2, h / 2 }; }
			constexpr RenderConfig::RenderOriginPoint TOP_RIGHT(const float& w) { return { w, 0 }; }
			constexpr RenderConfig::RenderOriginPoint BOTTOM_LEFT(const float& h) { return { 0, h }; }
			constexpr RenderConfig::RenderOriginPoint BOTTOM_RIGHT(const float& w, const float& h) { return { w, h }; }
			constexpr RenderConfig::RenderOriginPoint TOP_CENTRE(const float& w) { return { w / 2, 0 }; }
			constexpr RenderConfig::RenderOriginPoint BOTTOM_CENTRE(const float& w, const float& h) { return { w / 2, h }; }
			constexpr RenderConfig::RenderOriginPoint CENTRE_LEFT(const float& h) { return { 0, h / 2 }; }
			constexpr RenderConfig::RenderOriginPoint CENTRE_RIGHT(const float& w, const float& h) { return { w, h / 2 }; }
		}
	}
}