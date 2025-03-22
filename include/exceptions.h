#pragma once
#include <stb_image.h>
#include <format>
#include <stdexcept>
#include <SDL3/SDL_error.h>

namespace SDL_Exceptions
{
	struct SDL_Exception : std::runtime_error
	{
		explicit SDL_Exception(const std::string& message) : std::runtime_error(std::format("{} ({})", message, SDL_GetError())) {}
	};
	namespace Init
	{
		struct SDL_InitSDL_Failed : SDL_Exception
		{
			explicit SDL_InitSDL_Failed() : SDL_Exception("Initializing SDL failed") {}
		};
		struct SDL_CreateWindow_Failed : SDL_Exception
		{
			explicit SDL_CreateWindow_Failed() : SDL_Exception("Couldn't create window") {}
		};
		struct SDL_CreateRenderer_Failed : SDL_Exception
		{
			explicit SDL_CreateRenderer_Failed() : SDL_Exception("Couldn't create SDL renderer") {}
		};
	}
	namespace Texture
	{
		struct SDL_UpdateTexture_Failed : SDL_Exception
		{
			explicit SDL_UpdateTexture_Failed(const char* file_path) : SDL_Exception(std::format("Couldn't update img -> texture: {}", file_path)) {}
		};
		struct SDL_RenderTexture_Failed : SDL_Exception
		{
			explicit SDL_RenderTexture_Failed(const std::string& name) : SDL_Exception(std::format("Couldn't render texture: {}", name)) {}
		};
	}
}

namespace STBI_Exceptions
{
	struct STBI_Exception : std::runtime_error
	{
		explicit STBI_Exception(const std::string& message) : std::runtime_error(std::format("{} ({})", message, stbi_failure_reason())) {}
	};

	namespace Image
	{
		struct STBI_LoadImage_Failed : STBI_Exception
		{
			explicit STBI_LoadImage_Failed(const char* file_path) : STBI_Exception(std::format("Couldn't load image: {}", file_path)) {}
		};
	}
}
