#include "structures/render/objects/string.h" // Header
#include "utilities.hpp"
#include "structures/render/object.h"

namespace Structures::Render::Objects
{
	void HorizontalString::update()
	{
		if (!string || !alphabet) return;
		using namespace Utilities::Math::FPoint;

		for (size_t chr = 0; chr < string->size(); chr++)
		{
			const auto& character = alphabet->find(string->at(chr));
			if (character == alphabet->end()) continue;

			// Tạo object
			while (data.size() < string->size())
			{
				data.emplace_back(
					skin->find(character->second),
					Types::Render::OriginType::TOP_LEFT);
			}
			if (data[chr].src.get_name() != character->second)
				data[chr].src.change_target(character->second);

			// Set render character_render_size & pos
			data[chr].set_render_size(character_render_size);
			if (chr == 0) data[chr].config.render_pos = { .x = 0, .y = 0 };
			else data[chr].config.render_pos =
			{
				.x = data[chr - 1].config.render_pos.x + data[chr - 1].get_sdl_dst_rect().w,
				.y = data[chr - 1].config.render_pos.y
			};
		}
		while (data.size() > string->size()) data.pop_back();
	}
	void HorizontalString::render(const SDL_FPoint& offset)
	{
		using namespace Utilities::Math::FPoint;
		if (!string || !alphabet) return;
		this->update();

		const SDL_FPoint total_size = { character_render_size.x * static_cast<float>(data.size()), character_render_size.y };
		PolyObject::render(offset + render_pos - total_size * origin_point_in_percent);
	}
	HorizontalString::HorizontalString(
		const std::string* string,
		const TextureMemory* skin,
		const Alphabet* alphabet,
		const SDL_FPoint& render_pos,
		const SDL_FPoint& character_render_size,
		const Types::Render::OriginType& origin_type)
		: string(string), skin(skin), alphabet(alphabet),
		render_pos(render_pos),
		origin_point_in_percent(
			Object::Config::OriginPoint::translate_origin_type_to_point(origin_type, { .x = 1, .y = 1 })
		),
		character_render_size(character_render_size)
	{
	}
	HorizontalString::HorizontalString(
		const std::string* string,
		const TextureMemory* skin,
		const Alphabet* alphabet,
		const SDL_FPoint& render_pos,
		const SDL_FPoint& render_size,
		const SDL_FPoint& custom_origin_in_percent)
		: string(string), skin(skin), alphabet(alphabet),
		render_pos(render_pos), origin_point_in_percent(custom_origin_in_percent),
		character_render_size(render_size)
	{
	}
}
