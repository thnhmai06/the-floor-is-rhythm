#pragma once
#include "structures/render/collection.h"
#include "structures/render/objects/number.hpp"

namespace Structures::Screens::Gameplay::Playing::Score
{
	using namespace Structures::Render::Objects;

	struct Score : Collection
	{
		std::weak_ptr<HorizontalNumber<float>> score;
		std::weak_ptr<HorizontalNumber<float>> accuracy;
		std::weak_ptr<HorizontalNumber<unsigned long>> combo;

		Score(const TextureMemory& skin, const float* score, const float* accuracy, const unsigned long* combo);
	};
}