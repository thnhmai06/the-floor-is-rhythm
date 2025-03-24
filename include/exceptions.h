#pragma once
#include <format>
#include <cerrno>
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
		struct SDL_LoadTexture_Failed : SDL_Exception
		{
			explicit SDL_LoadTexture_Failed(const char* file_path) : SDL_Exception(std::format("Couldn't load texture: {}", file_path)) {}
		};
		struct SDL_RenderTexture_Failed : SDL_Exception
		{
			explicit SDL_RenderTexture_Failed(const std::string& name) : SDL_Exception(std::format("Couldn't render texture: {}", name)) {}
		};
	}
	namespace Audio
	{
		
	}
}

namespace File_Exceptions
{
	struct File_Exception : std::runtime_error
	{
		explicit File_Exception(const char* file_path, const std::string& message) : std::runtime_error(std::format("{} ({}): {}", message, file_path , strerror(errno))) {}
	};
	struct File_Open_Failed : File_Exception
	{
		explicit File_Open_Failed(const char* file_path) : File_Exception(file_path, "Couldn't Open file") {}
	};
}
