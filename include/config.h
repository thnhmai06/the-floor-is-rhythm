﻿#pragma once
#include <cstdint>
#include <SDL3/SDL_audio.h>

namespace UserConfig
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

namespace GameConfig
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
		constexpr int32_t LOGICAL_WIDTH = 1280;
		constexpr int32_t LOGICAL_HEIGHT = 720;

		namespace Camera
		{
			constexpr int32_t DEFAULT_POS_X = 0;
			constexpr int32_t DEFAULT_POS_Y = 0;
			constexpr int32_t DEFAULT_SIZE_WIDTH = LOGICAL_WIDTH;
			constexpr int32_t DEFAULT_SIZE_HEIGHT = LOGICAL_HEIGHT;
		}
	}
	namespace HitObject
	{
		constexpr int32_t DEFAULT_POS_X = Video::Camera::DEFAULT_POS_X;
		constexpr int32_t DEFAULT_POS_Y = Video::Camera::DEFAULT_POS_Y;
		constexpr int32_t SIZE = 64; // pixel
		constexpr int32_t SLIDER_POINT_SIZE_WIDTH = SIZE / 4; // pixel
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
			constexpr float BASE_SPEED = static_cast<float>(HitObject::SIZE) * 10 / 1000; // pixel/ms
		}
	}
}
