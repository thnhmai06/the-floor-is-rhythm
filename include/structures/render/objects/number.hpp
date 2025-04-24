#pragma once
#include "structures/render/objects/string.h"
#include "utilities.hpp"

namespace Structures::Render::Objects
{
	template <typename NumberType>
	struct HorizontalNumber : HorizontalString
	{
	protected:
		std::string raw_string;

	public:
		const NumberType* number;
		NumberType multiply = 1;
		uint8_t zero_padding = 0;
		uint8_t decimal_format = 0;

		void update() override
		{
			if (!number) return;
			using namespace Utilities::Math::FPoint;

			const auto rounded_number = Utilities::Math::round(*number * multiply, decimal_format);
			const auto [integer_part, decimal_part] = Utilities::Math::decimal_separate(rounded_number);
			// Xử lý phần thập phân
			std::stringstream ss, decimal_ss;
			decimal_ss << std::fixed << std::setprecision(decimal_format) << decimal_part;
			const std::string decimal_str = decimal_ss.str().substr(1); // xóa '0' ở đầu
			// Làm string
			if (!header.empty()) ss << header;
			ss << std::fixed << std::setfill('0') << std::setw(zero_padding) << std::setprecision(0) << integer_part;
			ss << decimal_str;
			if (!footer.empty()) ss << footer;
			raw_string = ss.str();

			HorizontalString::update();
		}

		HorizontalNumber(
			const NumberType* number,
			const TextureMemory* skin,
			const Alphabet* alphabet,
			const SDL_FPoint& render_pos,
			const SDL_FPoint& character_render_size,
			const Types::Render::OriginType& origin_type = Types::Render::OriginType::CENTRE)
			: HorizontalString(&raw_string, skin, alphabet, render_pos,
				character_render_size, origin_type),
			number(number)
		{
		}
		HorizontalNumber(
			const NumberType* number,
			const TextureMemory* skin,
			const Alphabet* alphabet,
			const SDL_FPoint& render_pos,
			const SDL_FPoint& character_render_size,
			const Object::Config::OriginPoint& custom_origin)
			: HorizontalString(&raw_string, skin, alphabet, render_pos,
				character_render_size, custom_origin),
			number(number)
		{
		}
	};
}