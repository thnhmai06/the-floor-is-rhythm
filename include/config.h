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
	}
	namespace KeyBinding
	{
		inline SDL_Scancode k1 = SDL_SCANCODE_S;
		inline SDL_Scancode k2 = SDL_SCANCODE_D;
		inline bool is_click_key(const SDL_Scancode& key) { return key == k1 || key == k2; }
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
		constexpr int32_t HIT_OBJECT_SIZE = 64; // pixel
		constexpr int32_t SLIDER_POINT_SIZE = HIT_OBJECT_SIZE / 4; // pixel
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
			constexpr float BASE_PIXEL_SPEED = static_cast<float>(HitObject::HIT_OBJECT_SIZE) * 10 / 1000; // pixel/ms
		}
	}
}
