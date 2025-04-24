#include "structures/render/objects/string.h" // Header
#include "utilities.hpp"

namespace Structures::Render::Objects
{
	void HorizontalString::update()
	{
		if (!string || !alphabet) return;
		using namespace Utilities::Math::FPoint;

		const auto previous_total_width = character_render_size.x * static_cast<float>(data.size());
		for (size_t i = 0; i < string->size(); i++)
		{
			const auto& character = alphabet->find(string->at(i));
			if (character == alphabet->end()) continue;

			// Tạo object
			if (data.size() < i)
			{
				data.emplace_back(
					skin->find(character->second),
					Types::Render::OriginType::TOP_LEFT);
			}
			else if (data[i].src.get_name() != character->second)
				data[i].src.change_target(character->second);

			// Set render character_render_size & pos
			data[i].set_render_size(character_render_size);
			if (i == 0) data[i].config.render_pos = { .x = 0, .y = 0 };
			else data[i].config.render_pos =
			{
				.x = data[i - 1].config.render_pos.x + data[i - 1].get_sdl_dst_rect().w,
				.y = data[i - 1].config.render_pos.y
			};
		}
		while (data.size() > string->size()) data.pop_back();

		if (relative_origin && !data.empty())
		{
			const float origin_point_x_percent = origin_point.x / previous_total_width;
			origin_point.x = origin_point_x_percent * character_render_size.x * static_cast<float>(data.size());
		}
	}
	void HorizontalString::render(const SDL_FPoint& offset)
	{
		using namespace Utilities::Math::FPoint;
		if (!string || !alphabet) return;
		this->update();

		PolyObject::render(offset + render_pos - origin_point);
	}
	HorizontalString::HorizontalString(
		const std::string* string,
		const TextureMemory* skin,
		const Alphabet* alphabet,
		const SDL_FPoint& render_pos,
		const SDL_FPoint& character_render_size,
		const Types::Render::OriginType& origin_type,
		const bool relative_origin)
		: string(string), skin(skin), alphabet(alphabet),
		render_pos(render_pos), origin_point(origin_type, character_render_size),
		relative_origin(relative_origin), character_render_size(character_render_size)
	{
	}
	HorizontalString::HorizontalString(
		const std::string* string,
		const TextureMemory* skin,
		const Alphabet* alphabet,
		const SDL_FPoint& render_pos,
		const SDL_FPoint& render_size,
		const Object::Config::OriginPoint& custom_origin,
		const bool relative_origin)
		: string(string), skin(skin), alphabet(alphabet),
		render_pos(render_pos), origin_point(custom_origin),
		relative_origin(relative_origin), character_render_size(render_size)
	{
	}
}