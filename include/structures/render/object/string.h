#pragma once
#include "structures/type.hpp"
#include "structures/render/object.h"

namespace Structures::Render::Object
{
	using Types::Format::Skin::Alphabet;

	struct HorizontalString : Collection
	{
		const std::string* string;
		const Memory* skin;
		const Alphabet* alphabet;

		SDL_FPoint render_pos;
		SDL_FPoint origin_point_in_percent;
		SDL_FPoint character_render_size;
		std::string header = {}, footer = {};

		virtual void update();
		void on_before_render() override;
		void on_after_render() override;

		HorizontalString(
			const std::string* string,
			const Memory* skin,
			const Alphabet* alphabet,
			const SDL_FPoint& render_pos,
			const SDL_FPoint& character_render_size,
			const Types::Render::OriginType& origin_type = Types::Render::OriginType::Centre);
		HorizontalString(
			const std::string* string,
			const Memory* skin,
			const Alphabet* alphabet,
			const SDL_FPoint& render_pos,
			const SDL_FPoint& render_size,
			const SDL_FPoint& custom_origin_in_percent);
	};
}
