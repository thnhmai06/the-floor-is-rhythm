#pragma once
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_scancode.h>

namespace Config::UserConfig
{
	namespace Video
	{
		inline int32_t width = 1366;
		inline int32_t height = 768;
	}
	namespace Audio::Volume
	{
		inline float master = 0.8f;
		inline float music = 0.8f;
		inline float effects = 0.8f;
	}
	namespace Cursor
	{
		inline int32_t size = 48;
		inline int32_t direction_size = 32;
	}
	namespace KeyBinding
	{
		namespace Direction
		{
			inline SDL_Scancode up = SDL_SCANCODE_UP;
			inline SDL_Scancode down = SDL_SCANCODE_DOWN;
			inline SDL_Scancode left = SDL_SCANCODE_LEFT;
			inline SDL_Scancode right = SDL_SCANCODE_RIGHT;
			inline bool is_direction(const SDL_Scancode& key)
			{
				return key == up || key == down || key == left || key == right;
			}
		}
		namespace Click
		{
			inline SDL_Scancode k1 = SDL_SCANCODE_Z;
			inline SDL_Scancode k2 = SDL_SCANCODE_X;
			inline bool is_click(const SDL_Scancode& key)
			{
				return key == k1 || key == k2;
			}
		}
	}
}

namespace Config::GameConfig
{
	namespace General
	{
		constexpr char NAME[] = "The Floor is Rhythm";
	}
	namespace Audio
	{
		constexpr uint8_t MAX_CHANNELS = 8;
		constexpr int32_t SAMPLE_FREQUENCY = 48000;
		constexpr SDL_AudioFormat AUDIO_FORMAT = SDL_AUDIO_F32LE;
		constexpr int32_t CHANNELS_MONO = 1, CHANNELS_STEREO = 2;
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
				constexpr int32_t MISS = 400; // 0 - ngoài tầm này chưa được tính là đã bấm (https://osu.ppy.sh/wiki/en/Gameplay/Judgement/osu%21)
			}

			namespace Multiply
			{
				constexpr int32_t PERFECT = 6; // 300
				constexpr int32_t GOOD = 8; // 100
				constexpr int32_t BAD = 10; // 50
			}
		}
		namespace HP
		{
			constexpr float BASE = 0.2f;
			constexpr float EASIER_MULTIPLY = 0.02f;
			constexpr float HARDER_MULTIPLY = 0.04f;
		}
		namespace Velocity
		{
			constexpr float BASE_SPEED = static_cast<float>(HitObject::SIZE) * 10 / 1000; // pixel/ms
		}
	}
}
