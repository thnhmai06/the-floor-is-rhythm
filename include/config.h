#pragma once
#include <SDL3/SDL_audio.h>
#include "structures/type.hpp"
#include "structures/config.h"
#include "utilities.hpp"

namespace Config
{
	inline std::unique_ptr<Structures::Config::UserConfig> user_config;

	namespace Game
	{
		namespace General
		{
			constexpr auto NAME = "The Floor is Rhythm!";
			constexpr int64_t DELAY_RESUME = 500; // ms

			namespace Path
			{
				constexpr auto SKIN = "./assets";
				constexpr auto CONFIG = "./config.ini";
				constexpr auto BEATMAP = "./beatmap";
			}
		}
		namespace Audio
		{
			constexpr uint32_t MAX_CHANNELS = 64;
			constexpr uint32_t SAMPLE_FREQUENCY = 48000;
			constexpr SDL_AudioFormat AUDIO_FORMAT = SDL_AUDIO_F32LE;
			constexpr uint8_t CHANNELS_MONO = 1, CHANNELS_STEREO = 2;
			constexpr uint32_t BUFFER_SIZE = 256;
		}
		namespace Render
		{
			constexpr SDL_FPoint STORYBOARD_GRID_SIZE = { .x = 640, .y = 480 };
			constexpr SDL_FPoint STORYBOARD_GRID_WIDESCREEN_SIZE = { .x = 854, .y = STORYBOARD_GRID_SIZE.y };
			using Utilities::Math::FPoint::operator/;
			const SDL_FPoint STORYBOARD_GRID_CENTRE = Utilities::Math::centre({ .x = 0, .y = 0 }, STORYBOARD_GRID_SIZE);
			const SDL_FPoint STORYBOARD_GRID_WIDESCREEN_CENTRE = Utilities::Math::centre({ .x = 0, .y = 0 }, STORYBOARD_GRID_WIDESCREEN_SIZE);

			constexpr SDL_FPoint DEFAULT_POS = { .x = 0, .y = 0 };
			namespace Camera
			{
				constexpr SDL_FPoint DEFAULT_POS = Render::DEFAULT_POS;
			}
			namespace HitObject
			{
				float get_size();
				constexpr auto ORIGIN = Structures::Types::Render::OriginType::Centre;
				constexpr SDL_Color DON = { 235, 69, 44, 255 };
				constexpr SDL_Color KAT = { 67, 142, 172, 255 };
			}
			namespace Cursor
			{
				constexpr auto ORIGIN = Structures::Types::Render::OriginType::Centre;
				SDL_FPoint get_pos();

				namespace NoteScore
				{
					inline float get_size()
					{
						return user_config->gameplay.cursor.get_pixel_size();
					}
					// start
					constexpr int64_t TIME_FADE_IN = 150; // ms
					constexpr auto EASING_IN = Structures::Types::Render::EasingFunctionType::QuintOut;
					inline SDL_FPoint get_start_pos()
					{
						const auto cursor_pos = get_pos();
						return { .x = cursor_pos.x, .y = cursor_pos.y - user_config->gameplay.cursor.get_pixel_size() };
					}
					// mid
					constexpr int64_t TIME_STAY = 300; // ms

					// end
					constexpr int64_t TIME_FADE_OUT = TIME_FADE_IN; // ms
					constexpr auto EASING_OUT = EASING_IN;
					inline SDL_FPoint get_end_pos()
					{
						const auto cursor_pos = get_pos();
						return { .x = cursor_pos.x, .y = cursor_pos.y - (user_config->gameplay.cursor.get_pixel_size() + get_size() * 0.25f) };
					}
				}
			}
			namespace Health
			{
				SDL_FPoint get_size();
				constexpr SDL_FPoint POS = { .x = 0, .y = 0 };
				constexpr auto ORIGIN = Structures::Types::Render::OriginType::TopLeft;

				constexpr int64_t CHANGE_TIME = 200; // ms
				constexpr auto EASING = Structures::Types::Render::EasingFunctionType::QuartOut;
			}
			namespace Score
			{
				constexpr float ratio = 44.0f / 68.0f;
				SDL_FPoint get_pos();
				constexpr auto ORIGIN = Structures::Types::Render::OriginType::TopRight;
				SDL_FPoint get_character_size();
				constexpr uint8_t ZERO_PADDING = 7; // Max Score = 1m có 7 chữ số
				constexpr uint8_t DECIMAL_FORMAT = 0; // không có số thập phân

				namespace Accuracy
				{
					SDL_FPoint get_pos();
					constexpr auto ORIGIN = Structures::Types::Render::OriginType::TopRight;
					SDL_FPoint get_character_size();
					constexpr uint8_t ZERO_PADDING = 2; // thường ở phạm vi xx%
					constexpr uint8_t DECIMAL_FORMAT = 2; // 2 chữ số thập phân
					constexpr auto FOOTER = "%";
					constexpr float MULTIPLY = 100; // từ 0 -> 1 thành 0 -> 100
				}

				namespace Combo
				{
					SDL_FPoint get_pos();
					constexpr auto ORIGIN = Structures::Types::Render::OriginType::BottomLeft;
					SDL_FPoint get_character_size();
					constexpr auto FOOTER = "x";

				}
			}
			namespace Pausing
			{
				constexpr auto ORIGIN_CONTINUE = Structures::Types::Render::OriginType::Centre;
				constexpr auto ORIGIN_RETRY = Structures::Types::Render::OriginType::Centre;
				constexpr auto ORIGIN_BACK = Structures::Types::Render::OriginType::Centre;
				SDL_FPoint get_button_size();
				SDL_FPoint get_continue_pos();
				SDL_FPoint get_retry_pos();
				SDL_FPoint get_back_pos();

				constexpr int64_t TIME_FADE_IN = 700; // ms
				constexpr int64_t TIME_FADE_OUT = TIME_FADE_IN; // ms
			}
			namespace Result
			{
				constexpr int64_t DELAY = 3000; // ms
				constexpr auto EASING_IN_RESULT = Structures::Types::Render::EasingFunctionType::CubicOut;
				constexpr int64_t TIME_MOVE_IN_RESULT = 1000; // ms


				constexpr SDL_FPoint BACKGROUND_POS = { 0, 0 };
				constexpr auto ORIGIN = Structures::Types::Render::OriginType::TopLeft;
				constexpr auto ORIGIN_GRADE = Structures::Types::Render::OriginType::Centre;
				SDL_FPoint get_score_pos();
				SDL_FPoint get_accuracy_pos();
				SDL_FPoint get_combo_pos();
				SDL_FPoint get_perfect_image_pos();
				SDL_FPoint get_good_image_pos();
				SDL_FPoint get_bad_image_pos();
				SDL_FPoint get_mis_image_pos();
				SDL_FPoint get_perfect_count_pos();
				SDL_FPoint get_good_count_pos();
				SDL_FPoint get_bad_count_pos();
				SDL_FPoint get_miss_count_pos();
				SDL_FPoint get_full_combo_pos();
				SDL_FPoint get_grade_pos();

				float get_score_width();
				float get_accuracy_width();
				float get_combo_width();
				float get_image_size();
				float get_count_width();
				SDL_FPoint get_full_combo_size();
				float get_grade_size();
			}
			namespace Progress
			{
				constexpr auto ORIGIN = Structures::Types::Render::OriginType::BottomLeft;
				SDL_FPoint get_pos();
				SDL_FPoint get_full_size();
			}
		}
		namespace KeyBinding
		{
			namespace Pausing
			{
				constexpr SDL_Scancode KEY_PAUSE = SDL_SCANCODE_ESCAPE;
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
}
