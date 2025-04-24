#include "structures/screens/gameplay/playing/score.h" // Header
#include "format/skin.h"
#include "config.h"

namespace Structures::Screens::Gameplay::Playing::Score
{
	Score::Score(const TextureMemory& skin, const unsigned long* score, const float* accuracy)
	{
		data.reserve(2);

		// Score
		constexpr SDL_FPoint score_pos = { Config::GameConfig::Video::LOGICAL_WIDTH, 0 }; // góc phải màn hình
		constexpr auto score_origin_type = Types::Render::OriginType::TOP_RIGHT;
		constexpr SDL_FPoint score_character_size = {
			Config::GameConfig::Video::LOGICAL_WIDTH * 0.07f,
			Config::GameConfig::Video::LOGICAL_HEIGHT * 0.05f };
		auto c_score = std::make_shared<HorizontalNumber<unsigned long>>(
			score, &skin, &Format::Skin::Score::alphabet, score_pos, score_character_size, score_origin_type, true);
		c_score->zero_padding = 7; // Max Score = 1m có 7 chữ số
		this->score = c_score;
		data.emplace_back(std::move(c_score));

		// Accuracy
		constexpr SDL_FPoint accuracy_pos = { Config::GameConfig::Video::LOGICAL_WIDTH, score_character_size.y }; // góc phải màn hình, dưới score
		constexpr auto accuracy_origin_type = Types::Render::OriginType::TOP_RIGHT;
		constexpr SDL_FPoint accuracy_character_size = {
			Config::GameConfig::Video::LOGICAL_WIDTH * 0.03f,
			Config::GameConfig::Video::LOGICAL_HEIGHT * 0.02f };
		auto c_accuracy = std::make_shared<HorizontalNumber<float>>(
			accuracy, &skin, &Format::Skin::Score::alphabet, accuracy_pos, accuracy_character_size, accuracy_origin_type, true);
		c_accuracy->decimal_format = 2; // 2 chữ số thập phân
		c_accuracy->zero_padding = 2; // thường ở phạm vi xx%
		c_accuracy->multiply = 100; // từ 0 -> 1 thành 0 -> 100
		c_accuracy->footer = "%";
		data.emplace_back(std::move(c_accuracy));
	}
}