#pragma once
#include "structures/render/object/number.hpp"
#include "structures/screen/gameplay/score.h"

namespace Structures::Screen::Gameplay::Result
{
	using namespace Render::Object;

	struct Result : Collection
	{
		std::shared_ptr<Object> background;
		std::shared_ptr<Object> full_combo;
		std::shared_ptr<HorizontalNumber<float>> score;
		std::shared_ptr<HorizontalNumber<float>> accuracy;
		std::shared_ptr<HorizontalNumber<unsigned long>> combo;
		std::shared_ptr<Object> perfect_image;
		std::shared_ptr<Object> good_image;
		std::shared_ptr<Object> bad_image;
		std::shared_ptr<Object> miss_image;
		std::shared_ptr<HorizontalNumber<unsigned long>> perfect_count;
		std::shared_ptr<HorizontalNumber<unsigned long>> good_count;
		std::shared_ptr<HorizontalNumber<unsigned long>> bad_count;
		std::shared_ptr<HorizontalNumber<unsigned long>> miss_count;
		std::shared_ptr<StaticHorizontalString> grade;

		explicit Result(const Memory& skin, 
			const Score::Logic::Score& score, const Score::Render::Score& r_score);
	};
}