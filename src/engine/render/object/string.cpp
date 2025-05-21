#include "engine/render/object/string.h" // Header
#include "utilities.h"
#include "engine/render/object.h"

namespace Engine::Render::Object
{
	//! HorizontalString
	void HorizontalString::update()
	{
		if (!string || !alphabet) return;
		using namespace Utilities::Math::FPoint;

		while (data.size() > string->size()) data.pop_back();
		if (data.size() < string->size())
		{
			Memory::Item deft;
			for (const auto& chr : *string)
			{
				const auto it = alphabet->find(chr);
				if (it == alphabet->end()) continue;
				const auto texture = skin->find(it->second, true);
				if (!texture.is_valid()) continue;
				deft = texture;
			}
			if (!deft.is_valid()) return;
			while (data.size() < string->size())
				data.emplace_back(std::make_shared<Object>(deft, OriginPoint{ 0, 0 }));
		}

		auto current_character = data.begin();
		for (size_t chr = 0; chr < string->size(); chr++)
		{
			const auto texture_name = alphabet->find(string->at(chr));
			if (texture_name == alphabet->end())
			{
				++current_character;
				continue;
			}
			const auto texture = skin->find(texture_name->second);
			if (!texture.is_valid())
			{
				++current_character;
				continue;
			}

			const auto it = std::get<std::shared_ptr<Object>>(*current_character);
			if (it->src.get_name() != texture_name->second)
				it->src.change_target(texture_name->second);

			it->set_render_size(character_render_size);
			if (chr == 0)
				it->config.render_pos = { .x = 0, .y = 0 };
			else
			{
				// chỗ này chắc chắn size > 1 (và chr >= 1)
				const auto& previous = std::get<std::shared_ptr<Object>>(*(std::prev(current_character)));

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
					const auto padding = (total_width.value() - character_render_size.x) / static_cast<float>(string->size() - 1);

					it->config.render_pos =
					{
						.x = padding * static_cast<float>(chr),
						.y = previous->config.render_pos.y
					};
				}
			}
			++current_character;
		}
	}
	void HorizontalString::on_before_render()
	{
		using namespace Utilities::Math::FPoint;
		if (!string || !alphabet) return;
		this->update();

		const SDL_FPoint total_size = { character_render_size.x * static_cast<float>(data.size()), character_render_size.y };
		config.render_pos = render_pos - total_size * origin_point_in_percent;
	}
	void HorizontalString::on_after_render()
	{
		using namespace Utilities::Math::FPoint;
		if (!string || !alphabet) return;

		const SDL_FPoint total_size = { character_render_size.x * static_cast<float>(data.size()), character_render_size.y };
		config.render_pos = render_pos - total_size * origin_point_in_percent;
	}
	HorizontalString::HorizontalString(
		const std::string* string,
		const Memory* skin,
		const Alphabet* alphabet,
		const SDL_FPoint& render_pos,
		const SDL_FPoint& character_render_size,
		const Engine::Render::OriginType& origin_type)
		: string(string), skin(skin), alphabet(alphabet),
		render_pos(render_pos),
		origin_point_in_percent(
			OriginPoint::translate_origin_type_to_point(origin_type, { .x = 1, .y = 1 })
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

	//! StaticHorizontalString
	StaticHorizontalString::StaticHorizontalString(
		std::string string, const Memory* skin,
		const Alphabet* alphabet, const SDL_FPoint& render_pos,
		const SDL_FPoint& character_render_size,
		const Engine::Render::OriginType& origin_type)
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
