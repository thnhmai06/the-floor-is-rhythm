#include "structures/screen/gameplay/result.h" // Header
#include "config.h"

namespace Structures::Screen::Gameplay::Result
{
	Result::Result(
		const Memory& skin,
		const Score::Logic::Score& score,
		const Score::Render::Score& r_score)
	{
		using namespace ::Config::Game::Render::Result;
		// General
		background = std::make_shared<Object>(skin.find(Format::Skin::Image::Result::background), ORIGIN, BACKGROUND_POS);
		background->set_render_size(Utilities::Math::FPoint::to_float_point(::Config::user_config->graphic.window_size));

		full_combo = std::make_shared<Object>(skin.find(Format::Skin::Image::Result::full_combo), ORIGIN, get_full_combo_pos());
		full_combo->set_render_size(get_full_combo_size());

		this->score = std::make_shared<HorizontalNumber<float>>(*r_score.score.lock());
		this->score->render_pos = get_score_pos();
		this->score->total_width = get_score_width();
		this->score->character_render_size = ::Config::Game::Render::Score::get_character_size();

		this->accuracy = std::make_shared<HorizontalNumber<float>>(*r_score.accuracy.lock());
		this->accuracy->render_pos = get_accuracy_pos();
		this->accuracy->total_width = get_accuracy_width();
		this->accuracy->character_render_size = ::Config::Game::Render::Score::Accuracy::get_character_size();

		this->combo = std::make_shared<HorizontalNumber<unsigned long>>(*r_score.combo.lock());
		this->combo->render_pos = get_combo_pos();
		this->combo->total_width = get_combo_width();
		this->combo->character_render_size = ::Config::Game::Render::Score::Combo::get_character_size();

		this->grade = std::make_shared<StaticHorizontalString>(
			std::string{ score.accuracy.get_grade() }, &skin, &Format::Skin::Image::Result::alphabet, get_grade_pos(),
			SDL_FPoint{ .x = get_grade_size() , .y = get_grade_size() }, ORIGIN_GRADE);

		// Count
		this->good_count = std::make_shared<HorizontalNumber<unsigned long>>(
			score.accuracy.get_count_100(), &skin, &Format::Skin::Image::Score::alphabet,
			get_good_count_pos(), ::Config::Game::Render::Score::get_character_size(),
			Types::Render::OriginType::Centre);
		this->bad_count = std::make_shared<HorizontalNumber<unsigned long>>(
			score.accuracy.get_count_50(), &skin, &Format::Skin::Image::Score::alphabet,
			get_bad_count_pos(), ::Config::Game::Render::Score::get_character_size(),
			Types::Render::OriginType::Centre);
		this->miss_count = std::make_shared<HorizontalNumber<unsigned long>>(
			score.accuracy.get_count_miss(), &skin, &Format::Skin::Image::Score::alphabet,
			get_miss_count_pos(), ::Config::Game::Render::Score::get_character_size(),
			Types::Render::OriginType::Centre);
		this->perfect_count = std::make_shared<HorizontalNumber<unsigned long>>(
			score.accuracy.get_count_300(), &skin, &Format::Skin::Image::Score::alphabet,
			get_perfect_count_pos(), ::Config::Game::Render::Score::get_character_size(),
			Types::Render::OriginType::Centre);
		perfect_count->total_width = good_count->total_width
			= bad_count->total_width = miss_count->total_width = get_count_width();

		// Image
		perfect_image = std::make_shared<Object>(
			skin.find(Format::Skin::Image::HitObject::hit.at(Types::Game::Gameplay::NoteScore::Perfect)),
			ORIGIN, get_perfect_image_pos());
		good_image = std::make_shared<Object>(
			skin.find(Format::Skin::Image::HitObject::hit.at(Types::Game::Gameplay::NoteScore::Good)),
			ORIGIN, get_good_image_pos());
		bad_image = std::make_shared<Object>(
			skin.find(Format::Skin::Image::HitObject::hit.at(Types::Game::Gameplay::NoteScore::Bad)),
			ORIGIN, get_bad_image_pos());
		miss_image = std::make_shared<Object>(
			skin.find(Format::Skin::Image::HitObject::hit.at(Types::Game::Gameplay::NoteScore::Miss)),
			ORIGIN, get_mis_image_pos());
		perfect_image->set_render_size(get_image_size());
		good_image->set_render_size(get_image_size());
		bad_image->set_render_size(get_image_size());
		miss_image->set_render_size(get_image_size());

		// Collection
		data.emplace_back(background);
		data.emplace_back(full_combo);
		data.emplace_back(this->score);
		data.emplace_back(this->accuracy);
		data.emplace_back(this->combo);
		data.emplace_back(perfect_image);
		data.emplace_back(good_image);
		data.emplace_back(bad_image);
		data.emplace_back(miss_image);
		data.emplace_back(perfect_count);
		data.emplace_back(good_count);
		data.emplace_back(bad_count);
		data.emplace_back(miss_count);
		data.emplace_back(grade);
		offset = { static_cast<float>(::Config::user_config->graphic.window_size.x), 0};
	}
}
