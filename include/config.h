#pragma once
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_scancode.h>
#include "structures/type.hpp"
#include "utilities.hpp"

namespace Config::UserConfig
{
	namespace Video
	{
		constexpr SDL_Point WINDOW_SIZE = { 1366, 768 };
	}
	namespace Audio::Volume
	{
		inline float master = 0.7f;
		inline float music = 0.4f;
		inline float effects = 0.8f;
	}
	namespace Cursor
	{
		inline int32_t size = 48;
	}
	namespace KeyBinding
	{
		inline SDL_Scancode l1 = SDL_SCANCODE_D;
		inline SDL_Scancode l2 = SDL_SCANCODE_F;
		inline SDL_Scancode r1 = SDL_SCANCODE_J;
		inline SDL_Scancode r2 = SDL_SCANCODE_K;
		inline bool is_click_key(const SDL_Scancode& key) { return key == l1 || key == l2 || key == r1 || key == r2; }
		inline bool is_left_key(const SDL_Scancode& key) { return key == l1 || key == l2; }
		inline bool is_right_key(const SDL_Scancode& key) { return key == r1 || key == r2; }
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
		constexpr uint32_t MAX_CHANNELS = 512;
		constexpr uint32_t SAMPLE_FREQUENCY = 48000;
		constexpr SDL_AudioFormat AUDIO_FORMAT = SDL_AUDIO_F32LE;
		constexpr uint8_t CHANNELS_MONO = 1, CHANNELS_STEREO = 2;
		constexpr uint32_t BUFFER_SIZE = 256;
	}
	namespace Render
	{
		constexpr float LOGICAL_WIDTH = UserConfig::Video::WINDOW_SIZE.x;
		constexpr float LOGICAL_HEIGHT = UserConfig::Video::WINDOW_SIZE.y;

		constexpr SDL_FPoint STORYBOARD_GRID_SIZE = { 640, 480 };
		constexpr SDL_FPoint STORYBOARD_GRID_WIDESCREEN_SIZE = { 854, STORYBOARD_GRID_SIZE.y };
		using Utilities::Math::FPoint::operator/;
		const SDL_FPoint STORYBOARD_GRID_CENTRE = STORYBOARD_GRID_SIZE / 2;
		const SDL_FPoint STORYBOARD_GRID_WIDESCREEN_CENTRE = STORYBOARD_GRID_WIDESCREEN_SIZE / 2;

		namespace Camera
		{
			constexpr float DEFAULT_POS_X = 0;
			constexpr float DEFAULT_POS_Y = 0;
			constexpr float DEFAULT_SIZE_WIDTH = LOGICAL_WIDTH;
			constexpr float DEFAULT_SIZE_HEIGHT = LOGICAL_HEIGHT;
		}

		constexpr float DEFAULT_POS_X = Camera::DEFAULT_POS_X;
		constexpr float DEFAULT_POS_Y = Camera::DEFAULT_POS_Y;

		namespace HitObject
		{
			constexpr float SIZE = 64; // pixel
			constexpr auto ORIGIN = Structures::Types::Render::OriginType::Centre;
			constexpr SDL_Color DON = {235, 69, 44, 255};
			constexpr SDL_Color KAT = {67, 142, 172, 255};
		}
		namespace Cursor
		{
			constexpr float SIZE = HitObject::SIZE; // pixel
			constexpr auto ORIGIN = Structures::Types::Render::OriginType::Centre;
			constexpr float POS_X = LOGICAL_WIDTH / 2;
			constexpr float POS_Y = LOGICAL_HEIGHT / 2;
		}
		namespace Health
		{
			// 4:1
			constexpr float SIZE_W = LOGICAL_WIDTH * 0.4f;
			constexpr float SIZE_H = LOGICAL_WIDTH * 0.1f;
			constexpr float POS_X = 0;
			constexpr float POS_Y = 0;
			constexpr auto ORIGIN = Structures::Types::Render::OriginType::TopLeft;
		}
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
				constexpr int32_t MISS = 400; // 0 - ngoài tầm này chưa được tính là đã bấm (https://Osu.ppy.sh/wiki/en/Gameplay/Judgement/Osu%21)
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
			constexpr float BASE_ONE_BEAT_PIXEL_LENGTH = 175; // pixel/ms
		}
	}
}
