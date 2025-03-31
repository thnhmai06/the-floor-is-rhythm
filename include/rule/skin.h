#pragma once
#include "template.h"
#include "game/hitobject.h"

struct HitObjectSkinByDirection
{
	std::string FLOOR = "floor";
	struct Slider
	{
		std::string FOCUS = "sliderfocus";
		std::string BEGIN = "sliderbegin";
		std::string LINE = "sliderline";
		std::string POINT = "sliderpoint";
		std::string CURVE = "slidercurve";
		std::string END = "sliderend";
	} SLIDER;

	HitObjectSkinByDirection(const Template::Game::Direction::Direction& direction)
	{
		std::string direction_str;
		switch (direction)
		{
		case Template::Game::Direction::Direction::RIGHT:
			direction_str = "right";
			break;
		case Template::Game::Direction::Direction::UP:
			direction_str = "up";
			break;
		case Template::Game::Direction::Direction::DOWN:
			direction_str = "down";
			break;
		case Template::Game::Direction::Direction::LEFT:
			direction_str = "left";
			break;
		}
		direction_str.push_back('/');

		FLOOR = direction_str + FLOOR;
		SLIDER.FOCUS = direction_str + SLIDER.FOCUS;
		SLIDER.BEGIN = direction_str + SLIDER.BEGIN;
		SLIDER.LINE = direction_str + SLIDER.LINE;
		SLIDER.POINT = direction_str + SLIDER.POINT;
		SLIDER.CURVE = direction_str + SLIDER.CURVE;
		SLIDER.END = direction_str + SLIDER.END;
	}
};

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
	inline constexpr std::unordered_map<Template::Game::Direction::Direction, const HitObjectSkinByDirection> HitObject = {
			{Template::Game::Direction::Direction::RIGHT, HitObjectSkinByDirection(Template::Game::Direction::Direction::RIGHT)},
			{Template::Game::Direction::Direction::UP, HitObjectSkinByDirection(Template::Game::Direction::Direction::UP)},
			{Template::Game::Direction::Direction::DOWN, HitObjectSkinByDirection(Template::Game::Direction::Direction::DOWN)},
			{Template::Game::Direction::Direction::LEFT, HitObjectSkinByDirection(Template::Game::Direction::Direction::LEFT)}
	};
}
