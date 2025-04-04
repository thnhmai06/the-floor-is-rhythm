#pragma once
#include <format>
#include <cerrno>
#include <stdexcept>
#include <SDL3/SDL_error.h>

using std::format;

namespace SDLExceptions
{
	struct SDL_Exception : std::runtime_error
	{
		explicit SDL_Exception(const std::string& message) : std::runtime_error(format("{} ({})", message, SDL_GetError())) {}
	};
	namespace Video
	{
		struct SDL_Video_InitSDL_Failed : SDL_Exception
		{
			explicit SDL_Video_InitSDL_Failed() : SDL_Exception("Initializing SDL failed") {}
		};
		struct SDL_Video_CreateWindow_Failed : SDL_Exception
		{
			explicit SDL_Video_CreateWindow_Failed() : SDL_Exception("Couldn't create window") {}
		};
		struct SDL_Video_CreateRenderer_Failed : SDL_Exception
		{
			explicit SDL_Video_CreateRenderer_Failed() : SDL_Exception("Couldn't create SDL renderer") {}
		};
	}
	namespace Texture
	{
		struct SDL_Texture_Load_Failed : SDL_Exception
		{
			explicit SDL_Texture_Load_Failed(const char* file_path) : SDL_Exception(format("Couldn't load texture: {}", file_path)) {}
		};
		struct SDL_Texture_Render_Failed : SDL_Exception
		{
			explicit SDL_Texture_Render_Failed(const std::string& name) : SDL_Exception(format("Couldn't render texture: {}", name)) {}
		};
	}
	namespace Audio
	{
		struct SDL_Audio_CreateMixer_Failed : SDL_Exception
		{
			explicit SDL_Audio_CreateMixer_Failed() : SDL_Exception("Couldn't create audio mixer") {}
		};
		struct SDL_Audio_LoadMusic_Failed : SDL_Exception
		{
			explicit SDL_Audio_LoadMusic_Failed(const char* file_path) : SDL_Exception(format("Couldn't load Music: {}", file_path)) {}
		};
		struct SDL_Audio_LoadEffect_Failed : SDL_Exception
		{
			explicit SDL_Audio_LoadEffect_Failed(const char* file_path) : SDL_Exception(format("Couldn't load Effect: {}", file_path)) {}
		};
		struct SDL_Audio_PlayMusic_Failed : SDL_Exception
		{
			explicit SDL_Audio_PlayMusic_Failed(const std::string& name) : SDL_Exception(format("Couldn't play Music: {}", name)) {}
		};
		struct SDL_Audio_PlayEffect_Failed : SDL_Exception
		{
			explicit SDL_Audio_PlayEffect_Failed(const std::string& name) : SDL_Exception(format("Couldn't play Effect: {}", name)) {}
		};
	}
}
namespace FileExceptions
{
	struct File_Exception : std::runtime_error
	{
		explicit File_Exception(const char* file_path, const std::string& message) :
			std::runtime_error(format("{} ({}): {}", message, file_path , strerror(errno))) {}
	};
	struct File_Open_Failed : File_Exception
	{
		explicit File_Open_Failed(const char* file_path) : File_Exception(file_path, "Couldn't open file") {}
	};
}
namespace FormatExceptions
{
	struct Format_Exception : std::invalid_argument
	{
		explicit Format_Exception(const std::string& message, const std::string& line) : std::invalid_argument(format("{}:\n{}", message, line)) {}
	};

	namespace HitObjects
	{
		struct Format_HitObjects_NotEnoughtContent : Format_Exception
		{
			explicit Format_HitObjects_NotEnoughtContent(const std::string& line) : Format_Exception("Not enough Hitobject content", line) {}
		};
		struct Format_HitObjects_InvalidContent : Format_Exception
		{
			explicit Format_HitObjects_InvalidContent(const std::string& line) : Format_Exception("Invalid Hitobject content", line) {}
		};
	}
	namespace TimingPoints
	{
		struct Format_TimingPoints_NotEnoughtContent : Format_Exception
		{
			explicit Format_TimingPoints_NotEnoughtContent(const std::string& line) : Format_Exception("Not enough TimingPoint content", line) {}
		};
		struct Format_TimingPoints_InvalidContent : Format_Exception
		{
			explicit Format_TimingPoints_InvalidContent(const std::string& line) : Format_Exception("Invalid TimingPoint content", line) {}
		};
	}
}