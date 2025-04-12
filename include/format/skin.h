// ReSharper disable StringLiteralTypo
#pragma once
#include <array>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "structures/types.h"

namespace Format::SkinFormat
{
	const std::unordered_set<std::string_view> SUPPORT_IMAGE_EXTENSIONS = { ".png", ".jpg" };

	namespace Cursor
	{
		const std::string BODY = "cursor";
		const std::string TRAIL = "cursortrail";
		inline std::unordered_map<Structures::Types::Game::Direction::Direction, const std::string> DIRECTION = {
			{Structures::Types::Game::Direction::Direction::RIGHT, "right/direction"},
			{Structures::Types::Game::Direction::Direction::UP, "up/direction"},
			{Structures::Types::Game::Direction::Direction::DOWN, "down/direction"},
			{Structures::Types::Game::Direction::Direction::LEFT, "left/direction"}
		};
	}

	namespace HitObject
	{
		using HitObjectTypeBase = uint8_t;
		constexpr HitObjectTypeBase NUM_SKIN_TYPE = 7;

		enum class HitObjectType : HitObjectTypeBase
		{
			FLOOR = 0,
			SLIDER_FOCUS = 1,
			SLIDER_BEGIN = 2,
			SLIDER_LINE = 3,
			SLIDER_POINT = 4,
			SLIDER_CURVE = 5,
			SLIDER_END = 6
		};
		struct HitObjectDirectionSkinType : std::array<std::string, NUM_SKIN_TYPE>
		{
		protected:
			using VALUE = std::string;
			using BASE = std::array<VALUE, NUM_SKIN_TYPE>;

		private:
			VALUE direction_folder;
			static std::string get_direction_folder(const Structures::Types::Game::Direction::Direction& direction) {
				switch (direction)
				{
				case Structures::Types::Game::Direction::Direction::RIGHT:
					return "right/";
				case Structures::Types::Game::Direction::Direction::UP:
					return "up/";
				case Structures::Types::Game::Direction::Direction::DOWN:
					return "down/";
				case Structures::Types::Game::Direction::Direction::LEFT:
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
			explicit HitObjectDirectionSkinType(const Structures::Types::Game::Direction::Direction& direction) :
				BASE{},
				direction_folder(get_direction_folder(direction))
			{
				(*this)[HitObjectType::FLOOR] = direction_folder + "floor";
				(*this)[HitObjectType::SLIDER_FOCUS] = direction_folder + "sliderfocus";
				(*this)[HitObjectType::SLIDER_BEGIN] = direction_folder + "sliderbegin";
				(*this)[HitObjectType::SLIDER_LINE] = direction_folder + "sliderline";
				(*this)[HitObjectType::SLIDER_POINT] = direction_folder + "sliderpoint";
				(*this)[HitObjectType::SLIDER_CURVE] = direction_folder + "slidercurve";
				(*this)[HitObjectType::SLIDER_END] = direction_folder + "sliderend";
			}
		};
		struct HitObjectSkinType : std::array<HitObjectDirectionSkinType, Structures::Types::Game::Direction::NUM_DIRECTIONS>
		{
		protected:
			using VALUE = HitObjectDirectionSkinType;
			using BASE = std::array<VALUE, Structures::Types::Game::Direction::NUM_DIRECTIONS>;

		public:
			reference operator[] (const Structures::Types::Game::Direction::Direction& direction)
			{
				return BASE::operator[](static_cast<Structures::Types::Game::Direction::DirectionBase>(direction));
			}
			const_reference operator[] (const Structures::Types::Game::Direction::Direction& direction) const
			{
				return BASE::operator[](static_cast<Structures::Types::Game::Direction::DirectionBase>(direction));
			}
			explicit HitObjectSkinType() : BASE{
				HitObjectDirectionSkinType(Structures::Types::Game::Direction::Direction::RIGHT),
				HitObjectDirectionSkinType(Structures::Types::Game::Direction::Direction::UP),
				HitObjectDirectionSkinType(Structures::Types::Game::Direction::Direction::DOWN),
				HitObjectDirectionSkinType(Structures::Types::Game::Direction::Direction::LEFT)
			} {
			}
		};
		inline const HitObjectSkinType HitObjectSkin;
	}

	// Dùng string do khi convert string_view -> string insert thì kí tự đầu trở thành null char
	struct ImageNameStorage : std::unordered_set<std::string> { ImageNameStorage(); } const IMAGE_STORAGE;
	struct FolderNameStorage : std::unordered_set<std::string> { FolderNameStorage(); } const FOLDER_STORAGE;
}