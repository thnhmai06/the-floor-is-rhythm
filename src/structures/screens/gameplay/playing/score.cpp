#include "structures/screens/gameplay/playing/score.h" // Header
#include "format/skin.h"
#include "config.h"

namespace Structures::Screens::Gameplay::Playing::Score
{
	Score::Score(const TextureMemory& skin, const float* score, const float* accuracy, const unsigned long* combo)
	{
		data.reserve(3);

		//! Config
		constexpr float ratio = 44.0f / 68.0f;
		// Score
		constexpr SDL_FPoint score_pos = { Config::GameConfig::Video::LOGICAL_WIDTH, 0 }; // góc phải màn hình
		constexpr auto score_origin_type = Types::Render::OriginType::TOP_RIGHT;
		constexpr SDL_FPoint score_character_size = {
			Config::GameConfig::Video::LOGICAL_WIDTH * 0.025f,
			Config::GameConfig::Video::LOGICAL_WIDTH * 0.025f / ratio
		};
		// Accuracy
		constexpr SDL_FPoint accuracy_pos = { Config::GameConfig::Video::LOGICAL_WIDTH, score_character_size.y }; // góc phải màn hình, dưới score
		constexpr auto accuracy_origin_type = Types::Render::OriginType::TOP_RIGHT;
		constexpr SDL_FPoint accuracy_character_size = {
			Config::GameConfig::Video::LOGICAL_WIDTH * 0.015f,
			Config::GameConfig::Video::LOGICAL_WIDTH * 0.015f / ratio
		};
		// Combo
		constexpr SDL_FPoint combo_pos = { 0, Config::GameConfig::Video::LOGICAL_HEIGHT }; // góc dưới trái màn hình
		constexpr auto combo_origin_type = Types::Render::OriginType::BOTTOM_LEFT;
		constexpr SDL_FPoint combo_character_size = {
			Config::GameConfig::Video::LOGICAL_WIDTH * 0.025f,
			Config::GameConfig::Video::LOGICAL_WIDTH * 0.025f / ratio
		};

		//! Setup
		// Score
		auto c_score = std::make_shared<HorizontalNumber<float>>(
			score, &skin, &Format::Skin::Image::Score::alphabet, score_pos, score_character_size, score_origin_type);
		c_score->zero_padding = 7; // Max Score = 1m có 7 chữ số
		c_score->decimal_format = 0;
		this->score = c_score;
		data.emplace_back(std::move(c_score));

		// Accuracy
		auto c_accuracy = std::make_shared<HorizontalNumber<float>>(
			accuracy, &skin, &Format::Skin::Image::Score::alphabet, accuracy_pos, accuracy_character_size, accuracy_origin_type);
		c_accuracy->decimal_format = 2; // 2 chữ số thập phân
		c_accuracy->zero_padding = 2; // thường ở phạm vi xx%
		c_accuracy->multiply = 100; // từ 0 -> 1 thành 0 -> 100
		c_accuracy->footer = "%";
		data.emplace_back(std::move(c_accuracy));

		// Combo
		auto c_combo = std::make_shared<HorizontalNumber<unsigned long>>(
			combo, &skin, &Format::Skin::Image::Score::alphabet, combo_pos, combo_character_size, combo_origin_type);
		c_combo->footer = "x";
		this->combo = c_combo;
		data.emplace_back(std::move(c_combo));
	}
}