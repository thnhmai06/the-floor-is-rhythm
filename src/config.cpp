#include "config.h" // Header

namespace Config
{
	namespace Game
	{
		namespace Render
		{
			namespace HitObject
			{
				float get_size()
				{
					return user_config->gameplay.cursor.get_pixel_size();
				}
			}

			namespace Cursor
			{
				SDL_FPoint get_pos()
				{
					return Utilities::Math::centre(
						{ .x = 0, .y = 0 },
						Utilities::Math::FPoint::to_float_point(
							user_config->graphic.window_size));
				}
			}

			namespace Health
			{
				SDL_FPoint get_size()
				{
					const auto& window_size = user_config->graphic.window_size;
					return { .x = window_size.x * 0.4f, .y = window_size.x * 0.1f }; // 4:1
				}
			}

			namespace Score
			{
				SDL_FPoint get_pos()
				{
					return { .x = static_cast<float>(user_config->graphic.window_size.x), .y = 0 }; // góc phải trên màn hình
				}

				SDL_FPoint get_character_size()
				{
					const auto window_size = user_config->graphic.window_size;
					return { .x = window_size.x * 0.025f, .y = window_size.x * 0.025f / ratio };
				}

				namespace Combo
				{
					SDL_FPoint get_pos()
					{
						return { .x = 0, .y = static_cast<float>(user_config->graphic.window_size.y) }; // góc trái dưới màn hình
					}

					SDL_FPoint get_character_size()
					{
						const auto window_size = user_config->graphic.window_size;
						return { .x = window_size.x * 0.025f, .y = window_size.x * 0.025f / ratio };
					}
				}

				namespace Accuracy
				{
					SDL_FPoint get_pos()
					{
						return { .x = static_cast<float>(user_config->graphic.window_size.x), .y = Score::get_character_size().y }; // Góc phải dưới score
					}

					SDL_FPoint get_character_size()
					{
						const auto window_size = user_config->graphic.window_size;
						return { .x = window_size.x * 0.015f, .y = window_size.x * 0.015f / ratio };
					}
				}
			}

			namespace Pausing
			{
				SDL_FPoint get_button_size()
				{
					using Utilities::Math::FPoint::operator*;

					constexpr SDL_FPoint percent = { .x = 0.07f, .y = 0.12f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				}
				SDL_FPoint get_continue_pos()
				{
					using Utilities::Math::FPoint::operator*;

					constexpr SDL_FPoint percent = { .x = 0.5f, .y = 0.3 };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				}
				SDL_FPoint get_retry_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.5f, .y = 0.5f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				}
				SDL_FPoint get_back_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.5f, .y = 0.7f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				}
			}
		}
	}
}