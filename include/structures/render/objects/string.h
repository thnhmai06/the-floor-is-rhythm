#pragma once
#include "structures/render/object.h"
#include "structures/types.h"

namespace Structures::Render::Objects
{
	using Types::Format::Skin::Alphabet;

	struct HorizontalString : PolyObject
	{
		const std::string* string;
		const TextureMemory* skin;
		const Alphabet* alphabet;

		SDL_FPoint render_pos;
		Object::Config::OriginPoint origin_point;
		bool relative_origin = true;
		SDL_FPoint character_render_size;
		std::string header = {}, footer = {};

		virtual void update();
		void render(const SDL_FPoint& offset = { 0, 0 }) override;

		HorizontalString(
			const std::string* string,
			const TextureMemory* skin,
			const Alphabet* alphabet,
			const SDL_FPoint& render_pos,
			const SDL_FPoint& character_render_size,
			const Types::Render::OriginType& origin_type = Types::Render::OriginType::CENTRE,
			bool relative_origin = true);
		HorizontalString(
			const std::string* string,
			const TextureMemory* skin,
			const Alphabet* alphabet,
			const SDL_FPoint& render_pos,
			const SDL_FPoint& render_size,
			const Object::Config::OriginPoint& custom_origin,
			bool relative_origin = true);
	};
}