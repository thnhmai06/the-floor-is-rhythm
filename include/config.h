#pragma once
#include <cstdint>
#include <SDL3/SDL_audio.h>

namespace Config
{
	namespace Video
	{
		inline int32_t width = 1366;
		inline int32_t height = 768;
	}
	namespace Volume
	{
		inline int32_t master = 80;
		inline int32_t music = 80;
		inline int32_t hitsound = 80;
	}
	namespace Cursor
	{
		inline int32_t size = 48;
		inline int32_t direction_size = 32;
	}
}

namespace Immutable
{
	namespace General
	{
		constexpr char NAME[] = "The Floor is Rhythm";
	}
	namespace Audio
	{
		constexpr int32_t MAX_CHANNELS = 8;                // Số kênh phát hiệu ứng tối đa
		constexpr int32_t SAMPLE_FREQUENCY = 48000;        // Tần số mẫu âm thanh
		constexpr SDL_AudioFormat AUDIO_FORMAT = SDL_AUDIO_F32LE;        // Format âm thanh
		constexpr int32_t MONO = 1, STEREO = 2;             // Số kênh âm thanh: 1 hay 2
		constexpr int32_t BUFFER_SIZE = 256;               // Kích cỡ buffer
	}
	namespace Video
	{
		constexpr int32_t LOGICAL_WIDTH = 1280;
		constexpr int32_t LOGICAL_HEIGHT = 720;
	}
}
