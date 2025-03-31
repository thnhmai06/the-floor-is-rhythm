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

namespace ImmutableConfig
{
	namespace General
	{
		constexpr char NAME[] = "The Floor is Rhythm";
	}
	namespace Audio
	{
		constexpr int32_t MAX_CHANNELS = 8;
		constexpr int32_t SAMPLE_FREQUENCY = 48000;
		constexpr SDL_AudioFormat AUDIO_FORMAT = SDL_AUDIO_F32LE;
		constexpr int32_t MONO = 1, STEREO = 2;
		constexpr int32_t BUFFER_SIZE = 256;
	}
	namespace Video
	{
		constexpr int32_t LOGICAL_WIDTH = 640;
		constexpr int32_t LOGICAL_HEIGHT = 480;
		constexpr int32_t GAMEPLAY_WIDTH = 510;
		constexpr int32_t GAMEPLAY_HEIGHT = 384;
	}
	namespace Difficulty
	{
		namespace OD
		{
			namespace Base
			{
				constexpr int32_t PERFECT = 80; // 300
				constexpr int32_t GOOD = 140; // 100
				constexpr int32_t BAD = 200; // 50
			}

			namespace Multiply
			{
				constexpr int32_t PERFECT = 6; // 300
				constexpr int32_t GOOD = 8; // 100
				constexpr int32_t BAD = 10; // 50
			}
		}
		namespace Velocity
		{
			constexpr float BASE_SPEED = 100; // pixel/s
		}
	}
	namespace HitObject
	{
		constexpr SDL_FPoint DEFAULT_POS = { 0, 0 };
		constexpr int32_t SIZE_HEIGHT = 40;
		constexpr int32_t SLIDER_POINT_SIZE_WIDTH = 10;
	}
}
