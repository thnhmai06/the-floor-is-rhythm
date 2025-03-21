#pragma once
#include <format>
#include <stdexcept>
#include <SDL3/SDL_error.h>

namespace SDL_Exceptions
{
	struct SDL_Exception : std::runtime_error
	{
		explicit SDL_Exception(const std::string& message) : std::runtime_error(message + '\n' + SDL_GetError()) {}
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
		struct IMG_LoadTexture_Failed : SDL_Exception
		{
			explicit IMG_LoadTexture_Failed(const char* file_path) : SDL_Exception(std::format("Couldn't load texture: {}", file_path)) {}
		};
	}
}

