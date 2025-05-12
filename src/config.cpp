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
			namespace Progress
			{
				SDL_FPoint get_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0, .y = 1 }; // Góc dưới trái
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				}
				SDL_FPoint get_full_size()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 1, .y = 0.01f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				}
			}

			namespace Result
			{
				SDL_FPoint get_score_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.13f, .y = 0.04f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_accuracy_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.234f, .y = 0.644f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_combo_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.033f, .y = 0.644f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_perfect_image_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.08f, .y = 0.23f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_good_image_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.26f, .y = 0.23f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_bad_image_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.08f, .y = 0.37f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_mis_image_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.26f, .y = 0.37f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_perfect_count_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.11f, .y = 0.23f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_good_count_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.44f, .y = 0.23f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_bad_count_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.11f, .y = 0.37f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_miss_count_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.44f, .y = 0.37f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				SDL_FPoint get_full_combo_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.18f, .y = 0.75f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				}
				SDL_FPoint get_grade_pos()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.8567f, .y = 0.3071f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				}
				float get_score_width()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr float percent = 0.25f;
					return user_config->graphic.window_size.x * percent;
				};
				float get_accuracy_width()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr float percent = 0.09f;
					return user_config->graphic.window_size.x * percent;
				};
				float get_combo_width()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr float percent = 0.063f;
					return user_config->graphic.window_size.x * percent;
				};

				float get_image_size()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr float percent = 0.07f;
					return user_config->graphic.window_size.y * percent;
				};
				float get_count_width()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr float percent = 0.1f;
					return user_config->graphic.window_size.y * percent;
				}
				SDL_FPoint get_full_combo_size()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr SDL_FPoint percent = { .x = 0.23f, .y = 0.23f };
					return Utilities::Math::FPoint::to_float_point(user_config->graphic.window_size) * percent;
				};
				float get_grade_size()
				{
					using Utilities::Math::FPoint::operator*;
					constexpr float percent = 0.5f;
					return user_config->graphic.window_size.y * percent;
				}
			}
		}
	}
}