﻿#include "structures/render/object/string.h" // Header
#include "utilities.hpp"
#include "structures/render/object.h"

namespace Structures::Render::Object
{
	void HorizontalString::update()
	{
		if (!string || !alphabet) return;
		using namespace Utilities::Math::FPoint;

		for (size_t chr = 0; chr < string->size(); chr++)
		{
			const auto& character = alphabet->find(string->at(chr));
			if (character == alphabet->end()) continue;

			// Tạo object khi thiếu
			while (data.size() < string->size())
				data.emplace_back(std::make_shared<Object>(skin->find(character->second), Types::Render::OriginType::TopLeft));

			// Sửa object
			const auto& it = std::get<std::shared_ptr<Object>>(data[chr]);
			if (it->src.get_name() != character->second)
				it->src.change_target(character->second);

			// Set on_before_render character_render_size & pos
			it->set_render_size(character_render_size);
			if (chr == 0)
				it->config.render_pos = { .x = 0, .y = 0 };
			else
			{
				const auto& previous = std::get<std::shared_ptr<Object>>(data[chr - 1]);

				if (!total_width.has_value())
				{
					it->config.render_pos =
					{
						.x = previous->config.render_pos.x + character_render_size.x,
						.y = previous->config.render_pos.y
					};
				}
				else
				{
					// chỗ này chắc chắn size > 1
					const auto padding = (total_width.value() - character_render_size.x) / (string->size() - 1);

					it->config.render_pos =
					{
						.x = padding * chr,
						.y = previous->config.render_pos.y
					};
				}
			}
		}
		while (data.size() > string->size()) data.pop_back();
	}
	void HorizontalString::on_before_render()
	{
		using namespace Utilities::Math::FPoint;
		if (!string || !alphabet) return;
		this->update();

		const SDL_FPoint total_size = { character_render_size.x * static_cast<float>(data.size()), character_render_size.y };
		offset += render_pos - total_size * origin_point_in_percent;
	}
	void HorizontalString::on_after_render()
	{
		using namespace Utilities::Math::FPoint;
		if (!string || !alphabet) return;

		const SDL_FPoint total_size = { character_render_size.x * static_cast<float>(data.size()), character_render_size.y };
		offset -= render_pos - total_size * origin_point_in_percent;
	}
	HorizontalString::HorizontalString(
		const std::string* string,
		const Memory* skin,
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
		const Memory* skin,
		const Alphabet* alphabet,
		const SDL_FPoint& render_pos,
		const SDL_FPoint& character_render_size,
		const SDL_FPoint& custom_origin_in_percent)
		: string(string), skin(skin), alphabet(alphabet),
		render_pos(render_pos), origin_point_in_percent(custom_origin_in_percent),
		character_render_size(character_render_size)
	{
	}


	StaticHorizontalString::StaticHorizontalString(
		std::string string, const Memory* skin,
		const Alphabet* alphabet, const SDL_FPoint& render_pos,
		const SDL_FPoint& character_render_size,
		const Types::Render::OriginType& origin_type)
		: HorizontalString(&value, skin, alphabet, render_pos, character_render_size, origin_type),
		value(std::move(string))
	{
	}
	StaticHorizontalString::StaticHorizontalString(
		std::string string, const Memory* skin, 
		const Alphabet* alphabet, const SDL_FPoint& render_pos, 
		const SDL_FPoint& character_render_size,
		const SDL_FPoint& custom_origin_in_percent)
			: HorizontalString(&value, skin, alphabet, render_pos, character_render_size, custom_origin_in_percent),
		value(std::move(string))
	{
	}
}
