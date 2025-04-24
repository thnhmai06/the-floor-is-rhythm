#pragma once
#include "structures/render/collection.h"
#include "structures/render/objects/number.hpp"

namespace Structures::Screens::Gameplay::Playing::Score
{
	using namespace Structures::Render::Objects;

	struct Score : Collection
	{
		std::weak_ptr<HorizontalNumber<unsigned long>> score;
		std::weak_ptr<HorizontalNumber<float>> accuracy;

		Score(const TextureMemory& skin, const unsigned long* score, const float* accuracy);
	};
}