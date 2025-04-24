// ReSharper disable StringLiteralTypo
#include "format/skin.h" // Header
#include <ranges>

namespace Format::Skin
{
	namespace HitObject
	{
		//! HitObjectsInDirection
		std::string HitObjectsInDirection::get_direction_folder(
			const Structures::Types::Game::Direction::Direction& direction)
		{
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
		HitObjectsInDirection::reference HitObjectsInDirection::operator[](const HitObjectType& type)
		{
			return BASE::operator[](static_cast<uint8_t>(type));
		}
		HitObjectsInDirection::const_reference HitObjectsInDirection::operator[](const HitObjectType& type) const
		{
			return BASE::operator[](static_cast<uint8_t>(type));
		}
		HitObjectsInDirection::HitObjectsInDirection(const Structures::Types::Game::Direction::Direction& direction) :
			BASE{},
			direction_folder(get_direction_folder(direction))
		{
			(*this)[HitObjectType::FLOOR] = direction_folder + "floor";
			(*this)[HitObjectType::SLIDER_FOCUS] = direction_folder + "sliderfocus";
			(*this)[HitObjectType::SLIDER_LINE] = direction_folder + "sliderline";
			(*this)[HitObjectType::SLIDER_POINT] = direction_folder + "sliderpoint";
		}

		//! HitObjectSkins
		HitObjectSkins::reference HitObjectSkins::operator[](
			const Structures::Types::Game::Direction::Direction& direction)
		{
			return BASE::operator[](static_cast<Structures::Types::Game::Direction::DirectionBase>(direction));
		}
		HitObjectSkins::const_reference HitObjectSkins::operator[](
			const Structures::Types::Game::Direction::Direction& direction) const
		{
			return BASE::operator[](static_cast<Structures::Types::Game::Direction::DirectionBase>(direction));
		}
		HitObjectSkins::HitObjectSkins() : BASE{
				HitObjectsInDirection(Structures::Types::Game::Direction::Direction::RIGHT),
				HitObjectsInDirection(Structures::Types::Game::Direction::Direction::UP),
				HitObjectsInDirection(Structures::Types::Game::Direction::Direction::DOWN),
				HitObjectsInDirection(Structures::Types::Game::Direction::Direction::LEFT) }
		{
		}
	}

	//! Namespace
	ImageNamespace::ImageNamespace()
	{
		// Của Cursor
		insert(Cursor::body);
		insert(Cursor::trail);
		for (const auto& val : Cursor::direction_skin | std::views::values)
		{
			insert(val);
		}

		// Của HealthBar
		insert(HealthBar::background);
		insert(HealthBar::colour);

		// Của HitObjects
		for (const auto& direction_skin : HitObject::hit_objects_skin)
			for (const auto& skin : direction_skin)
				insert(skin);
	}
	FolderNamespace::FolderNamespace()
	{
		// Của Cursor
		for (const auto& val : Cursor::direction_skin | std::views::values)
		{
			if (const auto first_slash = val.find('/');
				first_slash != std::string_view::npos)
				insert(val.substr(0, first_slash));
		}

		// Của HitObjects
		for (const auto& val : image_namespace)
		{
			if (const auto first_slash = val.find('/');
				first_slash != std::string_view::npos)
				insert(val.substr(0, first_slash));
		}
	}
}
