#include "structures/screen/gameplay/score.h" // Header
#include "format/skin.h"
#include "config.h"

namespace Structures::Screen::Gameplay::Score
{
	namespace Render
	{
		Score::Score(const Memory& skin, const float* score, const float* accuracy, const unsigned long* combo)
		{
			data.reserve(3);

			//! Config
			constexpr float ratio = 44.0f / 68.0f;
			// Score
			constexpr SDL_FPoint score_pos = { Config::GameConfig::Render::LOGICAL_WIDTH, 0 }; // góc phải màn hình
			constexpr auto score_origin_type = Types::Render::OriginType::TopRight;
			constexpr SDL_FPoint score_character_size = {
				Config::GameConfig::Render::LOGICAL_WIDTH * 0.025f,
				Config::GameConfig::Render::LOGICAL_WIDTH * 0.025f / ratio
			};
			// Accuracy
			constexpr SDL_FPoint accuracy_pos = { Config::GameConfig::Render::LOGICAL_WIDTH, score_character_size.y }; // góc phải màn hình, dưới score
			constexpr auto accuracy_origin_type = Types::Render::OriginType::TopRight;
			constexpr SDL_FPoint accuracy_character_size = {
				Config::GameConfig::Render::LOGICAL_WIDTH * 0.015f,
				Config::GameConfig::Render::LOGICAL_WIDTH * 0.015f / ratio
			};
			// Combo
			constexpr SDL_FPoint combo_pos = { 0, Config::GameConfig::Render::LOGICAL_HEIGHT }; // góc dưới trái màn hình
			constexpr auto combo_origin_type = Types::Render::OriginType::BottomLeft;
			constexpr SDL_FPoint combo_character_size = {
				Config::GameConfig::Render::LOGICAL_WIDTH * 0.025f,
				Config::GameConfig::Render::LOGICAL_WIDTH * 0.025f / ratio
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

	namespace Logic
	{
		// Accuracy
		void Score::Accuracy::reset()
		{
			count_perfect = count_good = count_bad = count_miss = 0;
		}
		unsigned long Score::Accuracy::get_elapsed_objects_num() const
		{
			return count_perfect + count_good + count_bad + count_miss;
		}
		void Score::Accuracy::update(const Types::Game::Gameplay::NoteScore& score, const unsigned long& num)
		{
			// Thêm count
			using Types::Game::Gameplay::NoteScore;
			switch (score)
			{
			case NoteScore::Perfect: count_perfect += num; break;
			case NoteScore::Good: count_good += num; break;
			case NoteScore::Bad: count_bad += num; break;
			case NoteScore::Miss: count_miss += num; break;
			case NoteScore::Skip: break;
			}

			// Tính acc
			if (count_perfect + count_good + count_bad + count_miss == 0) // ban đầu
			{
				accuracy = 1; // 100%
				return;
			}

			const unsigned long total =
				count_perfect * static_cast<int16_t>(NoteScore::Perfect)
				+ count_good * static_cast<int16_t>(NoteScore::Good)
				+ count_bad * static_cast<int16_t>(NoteScore::Bad);
				const unsigned long total_if_perfect =
				(count_perfect + count_good + count_bad + count_miss) * static_cast<int16_t>(NoteScore::Perfect);
			accuracy = static_cast<float>(total) / static_cast<float>(total_if_perfect);
		}

		// Combo
		const unsigned long* Score::Combo::get_current_combo() const { return &current_combo; }
		const unsigned long* Score::Combo::get_max_combo() const { return &max_combo; }
		void Score::Combo::add_combo(const uint32_t& num)
		{
			current_combo += num;
			max_combo = std::max(max_combo, current_combo);
		}
		void Score::Combo::reset(const bool reset_max_combo)
		{
			current_combo = 0;
			if (reset_max_combo) max_combo = 0;
		}

		// Score
		Types::Game::Gameplay::NoteScore Score::get_floor_score(
			const Game::Beatmap::HitObjects::Floor& floor,
			uint16_t& click_num,
			const int64_t& current_time) const
		{
			const auto range_bad = mapset->calculated_difficulty->overall_difficulty.get_bad();
			const auto range_miss = Game::Beatmap::Metadata::CalculatedDifficulty::OverallDifficulty::get_miss();

			if (static_cast<float>(current_time) < static_cast<float>(floor.time) - range_miss) // chưa đến
				return Types::Game::Gameplay::NoteScore::Skip;
			if (static_cast<float>(current_time) > static_cast<float>(floor.time) + range_bad) // quá muộn
				return Types::Game::Gameplay::NoteScore::Miss;

			const auto score = mapset->calculated_difficulty->overall_difficulty.get_score(click_num > 0, current_time, floor.time);
			click_num--;
			return score;
		}
		void Score::update(const Types::Game::Gameplay::NoteScore& note_score, const uint32_t& num)
		{
			// Cập nhật thuộc tính
			const auto& stats = mapset->stats;
			if (note_score != Types::Game::Gameplay::NoteScore::Skip)
			{
				accuracy.update(note_score, num);
				if (note_score == Types::Game::Gameplay::NoteScore::Miss)
					combo.reset();
				else combo.add_combo(num);
			}

			// Tính điểm
			// Score = ((700000 * max_combo / total_combo) + (300000 * accuracy ^ 10 * elapsed_objects / total_objects)) * mod_multiplier
			constexpr float BASE_COMBO = 700000;
			constexpr float BASE_ACCURACY = 300000;

			const float elapsed_objects = static_cast<float>(accuracy.get_elapsed_objects_num());
			const float combo_score = BASE_COMBO * static_cast<float>(*combo.get_max_combo()) / static_cast<float>(stats.count.total_combo);
			const float accuracy_score = BASE_ACCURACY * std::powf(*accuracy.get_accuracy(), 10) * elapsed_objects / static_cast<float>(stats.get_total_objects_num());
			const float score_no_multiplier = combo_score + accuracy_score;
			this->score = score_no_multiplier * *mod_multiplier;
		}
		const float* Score::get_score() const { return &score; }
		const float* Score::get_mod_multiplier() const { return mod_multiplier; }
		Score::Score(const Game::Beatmap::Mapset& mapset, const float* mod_multiplier)
			: mapset(&mapset), mod_multiplier(mod_multiplier)
		{
		}
	}
}