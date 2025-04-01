#pragma once
#include <array>
#include "template.h"
#include "game/hitobject.h"

namespace SkinFormat
{
	namespace Cursor
	{
		const std::string BODY = "cursor";
		const std::string TAIL = "cursortail";
		inline std::unordered_map<Template::Game::Direction::Direction, const std::string> DIRECTION = {
			{Template::Game::Direction::Direction::RIGHT, "right"},
			{Template::Game::Direction::Direction::UP, "up"},
			{Template::Game::Direction::Direction::DOWN, "down"},
			{Template::Game::Direction::Direction::LEFT, "left"}
		};
	}

	namespace HitObject
	{
		using HitObjectTypeBase = uint8_t;
		constexpr HitObjectTypeBase NUM_SKIN_TYPE = 6;
		
		enum class HitObjectType : HitObjectTypeBase
		{
			FLOOR,
			SLIDER_FOCUS,
			SLIDER_BEGIN,
			SLIDER_LINE,
			SLIDER_POINT,
			SLIDER_CURVE,
			SLIDER_END
		};
		class HitObjectDirectionSkinType : std::array<std::string, NUM_SKIN_TYPE>
		{
		protected:
			using VALUE = std::string;
			using BASE = std::array<VALUE, NUM_SKIN_TYPE>;

		private:
			static std::string get_direction_folder(const Template::Game::Direction::Direction& direction) {
				switch (direction)
				{
				case Template::Game::Direction::Direction::RIGHT:
					return "right/";
				case Template::Game::Direction::Direction::UP:
					return "up/";
				case Template::Game::Direction::Direction::DOWN:
					return "down/";
				case Template::Game::Direction::Direction::LEFT:
					return "left/";
				}
				return "right/";
			}

		public:
			reference operator[] (const HitObjectType& type)
			{
				return BASE::operator[](static_cast<HitObjectTypeBase>(type));
			}
			const_reference operator[] (const HitObjectType& type) const
			{
				return BASE::operator[](static_cast<HitObjectTypeBase>(type));
			}
			explicit HitObjectDirectionSkinType(const Template::Game::Direction::Direction& direction) : BASE()
			{
				// hehe xd
				const auto direction_folder = get_direction_folder(direction);
				(*this)[HitObjectType::FLOOR] = direction_folder + "floor";
				(*this)[HitObjectType::SLIDER_FOCUS] = direction_folder + "sliderfocus";
				(*this)[HitObjectType::SLIDER_BEGIN] = direction_folder + "sliderbegin";
				(*this)[HitObjectType::SLIDER_LINE] = direction_folder + "sliderline";
				(*this)[HitObjectType::SLIDER_POINT] = direction_folder + "sliderpoint";
				(*this)[HitObjectType::SLIDER_CURVE] = direction_folder + "slidercurve";
				(*this)[HitObjectType::SLIDER_END] = direction_folder + "sliderend";
			}
		};
		class HitObjectSkinType : std::array<HitObjectDirectionSkinType, Template::Game::Direction::NUM_DIRECTIONS>
		{
		protected:
			using VALUE = HitObjectDirectionSkinType;
			using BASE = std::array<VALUE, Template::Game::Direction::NUM_DIRECTIONS>;

		public:
			reference operator[] (const Template::Game::Direction::Direction& direction)
			{
				return BASE::operator[](static_cast<Template::Game::Direction::DirectionBase>(direction));
			}
			const_reference operator[] (const Template::Game::Direction::Direction& direction) const
			{
				return BASE::operator[](static_cast<Template::Game::Direction::DirectionBase>(direction));
			}
			explicit HitObjectSkinType() : BASE()
			{
				(*this)[Template::Game::Direction::Direction::RIGHT] = HitObjectDirectionSkinType(Template::Game::Direction::Direction::RIGHT);
				(*this)[Template::Game::Direction::Direction::UP] = HitObjectDirectionSkinType(Template::Game::Direction::Direction::UP);
				(*this)[Template::Game::Direction::Direction::DOWN] = HitObjectDirectionSkinType(Template::Game::Direction::Direction::DOWN);
				(*this)[Template::Game::Direction::Direction::LEFT] = HitObjectDirectionSkinType(Template::Game::Direction::Direction::LEFT);
			}
		};
		inline const HitObjectSkinType HitObjectSkin;
	}
}