#pragma once
#include <format>
#include <cerrno>
#include <stdexcept>
#include <filesystem>
#include <SDL3/SDL_error.h>

using std::format;

namespace Logging::Exceptions::SDLExceptions
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
		struct SDL_Texture_Create_Failed : SDL_Exception
		{
			explicit SDL_Texture_Create_Failed(const std::string& name) : SDL_Exception(format("Couldn't create texture: {}", name)) {}
		};
		struct SDL_Texture_Load_Failed : SDL_Exception
		{
			explicit SDL_Texture_Load_Failed(const std::filesystem::path& file_path) : SDL_Exception(format("Couldn't load texture: {}", file_path.string())) {}
		};
		struct SDL_Texture_Render_Failed : SDL_Exception
		{
			explicit SDL_Texture_Render_Failed(const std::string& name) : SDL_Exception(format("Couldn't render texture: {}", name)) {}
		};
		struct SDL_Texture_NotFound : SDL_Exception
		{
			explicit SDL_Texture_NotFound(const std::string& name) : SDL_Exception(format("Texture not found: {}", name)) {}
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
			explicit SDL_Audio_LoadMusic_Failed(const std::filesystem::path& file_path) : SDL_Exception(format("Couldn't load Music: {}", file_path.string())) {}
		};
		struct SDL_Audio_LoadEffect_Failed : SDL_Exception
		{
			explicit SDL_Audio_LoadEffect_Failed(const std::filesystem::path& file_path) : SDL_Exception(format("Couldn't load Effect: {}", file_path.string())) {}
		};
		struct SDL_Audio_PlayMusic_Failed : SDL_Exception
		{
			explicit SDL_Audio_PlayMusic_Failed(const std::string& name) : SDL_Exception(format("Couldn't play Music: {}", name)) {}
		};
		struct SDL_Audio_PlayEffect_Failed : SDL_Exception
		{
			explicit SDL_Audio_PlayEffect_Failed(const std::string& name) : SDL_Exception(format("Couldn't play Effect: {}", name)) {}
		};
		struct SDL_Audio_NotFound : std::runtime_error
		{
			explicit SDL_Audio_NotFound(const std::string& name) : std::runtime_error(format("Audio not found: {}", name)) {}
		};
		struct SDL_Audio_SetMusicPosition_Failed : SDL_Exception
		{
			explicit SDL_Audio_SetMusicPosition_Failed(double new_pos_sec) : SDL_Exception(format("Couldn't set music to position: {}", new_pos_sec)) {}
		};
	}
}
namespace Logging::Exceptions::FileExceptions
{
	struct File_Exception : std::runtime_error
	{
		explicit File_Exception(const std::filesystem::path& file_path, const std::string& message) :
			std::runtime_error(format("{} ({}): {}", message, file_path.string(), strerror(errno))) {}
	};
	struct File_Open_Failed : File_Exception
	{
		explicit File_Open_Failed(const std::filesystem::path& file_path) : File_Exception(file_path, "Couldn't open file") {}
	};
}
namespace Logging::Exceptions::FormatExceptions
{
	struct Format_Exception : std::invalid_argument
	{
		explicit Format_Exception(const std::string& message, const std::string& line) : std::invalid_argument(format("{}:\n{}", message, line)) {}
	};

	namespace HitObjects
	{
		struct Format_HitObjects_NotEnoughContent : Format_Exception
		{
			explicit Format_HitObjects_NotEnoughContent(const std::string& line) : Format_Exception("Not enough HitObject content", line) {}
		};
		struct Format_HitObjects_InvalidContent : Format_Exception
		{
			explicit Format_HitObjects_InvalidContent(const std::string& line) : Format_Exception("Invalid HitObject content", line) {}
		};
	}
	namespace TimingPoints
	{
		struct Format_TimingPoints_NotEnoughContent : Format_Exception
		{
			explicit Format_TimingPoints_NotEnoughContent(const std::string& line) : Format_Exception("Not enough TimingPoint content", line) {}
		};
		struct Format_TimingPoints_InvalidContent : Format_Exception
		{
			explicit Format_TimingPoints_InvalidContent(const std::string& line) : Format_Exception("Invalid TimingPoint content", line) {}
		};
	}
}