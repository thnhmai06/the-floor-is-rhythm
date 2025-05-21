#pragma once
#include "engine/render/object.h"

namespace Engine::Render::Object
{
	struct HorizontalString : Collection
	{
		const std::string* string;
		const Memory* skin;
		const Alphabet* alphabet;

		SDL_FPoint render_pos;
		SDL_FPoint origin_point_in_percent;
		SDL_FPoint character_render_size;
		std::optional<float> total_width;
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
			const OriginType& origin_type = OriginType::Centre);
		HorizontalString(
			const std::string* string,
			const Memory* skin,
			const Alphabet* alphabet,
			const SDL_FPoint& render_pos,
			const SDL_FPoint& character_render_size,
			const SDL_FPoint& custom_origin_in_percent);
	};

	struct StaticHorizontalString : HorizontalString
	{
		std::string value;

		StaticHorizontalString(
			std::string string,
			const Memory* skin,
			const Alphabet* alphabet,
			const SDL_FPoint& render_pos,
			const SDL_FPoint& character_render_size,
			const OriginType& origin_type = OriginType::Centre);
		StaticHorizontalString(
			std::string string,
			const Memory* skin,
			const Alphabet* alphabet,
			const SDL_FPoint& render_pos,
			const SDL_FPoint& character_render_size,
			const SDL_FPoint& custom_origin_in_percent);
	};
}
