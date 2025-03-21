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
			explicit SDL_InitSDL_Failed() : SDL_Exception("Initializing Init failed") {}
		};
		struct SDL_CreateWindow_Failed : SDL_Exception
		{
			explicit SDL_CreateWindow_Failed() : SDL_Exception("Creating window failed") {}
		};
		struct SDL_CreateGPUDevice_Failed : SDL_Exception
		{
			explicit SDL_CreateGPUDevice_Failed() : SDL_Exception("Creating GPU device failed") {}
		};
		struct SDL_CreateRenderer_Failed : SDL_Exception
		{
			explicit SDL_CreateRenderer_Failed() : SDL_Exception("Creating renderer failed") {}
		};
	}
	namespace Texture
	{
		struct SDL_CouldntCreateTexture : SDL_Exception
		{
			explicit SDL_CouldntCreateTexture(const char* file_name) : SDL_Exception(std::format("{}: {}", "Couldn't create texture", file_name)) {}
		};
		struct SDL_CouldntRenderTexture : SDL_Exception
		{
			explicit SDL_CouldntRenderTexture() : SDL_Exception("Couldn't render texture") {}
		};
	};
}

