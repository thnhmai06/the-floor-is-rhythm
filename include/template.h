#pragma once
#include <concepts>
#include <SDL3_mixer/SDL_mixer.h>
#include "render/texture.h"

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
			inline constexpr Texture::TextureConfig::TextureOrigin TOP_LEFT = { 0, 0 };
			constexpr Texture::TextureConfig::TextureOrigin CENTRE(const float& w, const float& h) { return { w / 2, h / 2 }; }
			constexpr Texture::TextureConfig::TextureOrigin TOP_RIGHT(const float& w) { return { w, 0 }; }
			constexpr Texture::TextureConfig::TextureOrigin BOTTOM_LEFT(const float& h) { return { 0, h }; }
			constexpr Texture::TextureConfig::TextureOrigin BOTTOM_RIGHT(const float& w, const float& h) { return { w, h }; }
			constexpr Texture::TextureConfig::TextureOrigin TOP_CENTRE(const float& w) { return { w / 2, 0 }; }
			constexpr Texture::TextureConfig::TextureOrigin BOTTOM_CENTRE(const float& w, const float& h) { return { w / 2, h }; }
			constexpr Texture::TextureConfig::TextureOrigin CENTRE_LEFT(const float& h) { return { 0, h / 2 }; }
			constexpr Texture::TextureConfig::TextureOrigin CENTRE_RIGHT(const float& w, const float& h) { return { w, h / 2 }; }
		}
	}
}