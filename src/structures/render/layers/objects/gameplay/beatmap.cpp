﻿#include "structures/render/layers/objects/gameplay/beatmap.h" // Header
#include <ranges>
#include "format/skin.h"
#include "config.h"
#include "structures/types.h"
#include "utilities.h"

namespace Structures::Render::Objects::Gameplay::Beatmap
{
	static auto current_direction = Types::Game::Direction::Direction::RIGHT;
	static SDL_FPoint translate_width_based_on_direction(const float& width, const float& default_size = Config::GameConfig::HitObject::SIZE)
	{
		SDL_FPoint size = { default_size, default_size };
		switch (current_direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
		case Types::Game::Direction::Direction::LEFT:
			size.x = width;
			break;

		case Types::Game::Direction::Direction::UP:
		case Types::Game::Direction::Direction::DOWN:
			size.y = width;
			break;
		}

		return size;
	}

	//! ::Components
	using namespace Components;
	using Format::SkinFormat::HitObject::HitObjectType, Format::SkinFormat::HitObject::HitObjectSkin;
	// ::HitObject
	//? static
	Object HitObject::create_spacing_object(
		const TextureMemory::Item& texture,
		const Game::Beatmap::HitObjects::HitObject& current,
		const float& speed,
		const HitObject* previous)
	{
		Object object(texture, Types::Render::RenderOriginType::CENTRE);
		// Pos
		if (!previous || !previous->hit_object)
		{
			current_direction = Types::Game::Direction::Direction::RIGHT + current.get_rotation(); // reset
			object.config.render_pos =
				SDL_FPoint{
					Config::GameConfig::HitObject::DEFAULT_POS_X,
					Config::GameConfig::HitObject::DEFAULT_POS_Y
			};
		}
		else
		{
			const float time_distance = static_cast<float>(current.get_time() - previous->hit_object->get_end_time());
			object.config.render_pos = previous->back().config.render_pos;
			switch (current_direction)
			{
			case Types::Game::Direction::Direction::RIGHT:
				object.config.render_pos.x += speed * time_distance;
				break;

			case Types::Game::Direction::Direction::UP:
				object.config.render_pos.y -= speed * time_distance;
				break;

			case Types::Game::Direction::Direction::LEFT:
				object.config.render_pos.x -= speed * time_distance;
				break;

			case Types::Game::Direction::Direction::DOWN:
				object.config.render_pos.y += speed * time_distance;
				break;
			}
		}
		object.set_render_size(Config::GameConfig::HitObject::SIZE);

		return object;
	}
	//? public
	HitObject::HitObject(
		const TextureMemory::Item& texture,
		const Game::Beatmap::HitObjects::HitObject& current,
		const float& speed,
		const HitObject* previous) : hit_object(&current)
	{
		push_back(create_spacing_object(texture, current, speed, previous));
	}

	// ::Floor
	Floor::Floor(
		const Game::Beatmap::HitObjects::HitObject& floor_hit_object,
		const TextureMemory& memory,
		const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
		const float& current_timing_velocity,
		const HitObject* previous) :
		HitObject(memory.find(HitObjectSkin[current_direction += floor_hit_object.get_rotation()][HitObjectType::FLOOR]),
			floor_hit_object, current_timing_velocity* diff.velocity.speed, previous)
	{
	}

	// ::Slider
	//? Static
	Object Slider::create_adjacent_object(
		const TextureMemory::Item& texture,
		const SDL_FPoint& size,
		const Object& previous,
		float src_width_in_percent,
		const bool src_from_beginning)
	{
		Object object(texture, Types::Render::RenderOriginType::CENTRE);
		object.config.render_pos = previous.config.render_pos;
		float src_x_in_percent = 0;
		// size
		object.set_render_size(size);
		// pos
		if (!src_from_beginning)
		{
			src_x_in_percent = 1 - src_width_in_percent;
			src_width_in_percent = 1 - src_width_in_percent;
		}
		// Chia 2 vì vị trí đặt vào màn hình là centre của object
		switch (current_direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
			object.src_rect_in_percent.x = src_x_in_percent;
			object.src_rect_in_percent.w = src_width_in_percent;
			object.config.render_pos.x += previous.get_render_size().x / 2 + object.get_render_size().x / 2;
			//object.config.render_pos.y += 10; // for debugging
			break;
		case Types::Game::Direction::Direction::UP:
			object.src_rect_in_percent.y = src_x_in_percent;
			object.src_rect_in_percent.h = src_width_in_percent;
			object.config.render_pos.y -= previous.get_render_size().y / 2 + object.get_render_size().y / 2;
			break;
		case Types::Game::Direction::Direction::LEFT:
			object.src_rect_in_percent.x = src_x_in_percent;
			object.src_rect_in_percent.w = src_width_in_percent;
			object.config.render_pos.x -= previous.get_render_size().x / 2 + object.get_render_size().x / 2;
			break;
		case Types::Game::Direction::Direction::DOWN:
			object.src_rect_in_percent.y = src_x_in_percent;
			object.src_rect_in_percent.h = src_width_in_percent;
			object.config.render_pos.y += previous.get_render_size().y / 2 + object.get_render_size().y / 2;
			break;
		}

		return object;
	}
	Object Slider::create_slider_point(
		const TextureMemory::Item& texture,
		const Object& previous)
	{
		Object current(texture, Types::Render::RenderOriginType::CENTRE);
		current.config.render_pos.x = previous.config.render_pos.x;
		current.config.render_pos.y = previous.config.render_pos.y;
		SDL_FPoint size = { Config::GameConfig::HitObject::SIZE, Config::GameConfig::HitObject::SIZE };
		switch (current_direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
			size.x = Config::GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
			current.config.render_pos.x = previous.get_render_sdl_pos().x + previous.get_render_size().x + size.x / 2;
			break;

		case Types::Game::Direction::Direction::LEFT:
			size.x = Config::GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
			current.config.render_pos.x = previous.get_render_sdl_pos().x - size.x / 2;
			break;

		case Types::Game::Direction::Direction::UP:
			size.y = Config::GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
			current.config.render_pos.y = previous.get_render_sdl_pos().y - size.y / 2;
			break;

		case Types::Game::Direction::Direction::DOWN:
			size.y = Config::GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
			current.config.render_pos.y = previous.get_render_sdl_pos().y + previous.get_render_size().y + size.y / 2;
			break;
		}
		current.set_render_size(size);
		return current;
	}
	//? Public
	Slider::Slider(
		const Game::Beatmap::HitObjects::HitObject& slider_object,
		const TextureMemory& memory,
		const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
		const float& current_beat_length,
		const float& current_timing_velocity,
		const HitObject* previous) :
		HitObject()
	{
		hit_object = &slider_object;
		const auto slider = *std::get_if<Game::Beatmap::HitObjects::Slider>(&slider_object);
		const auto speed = current_timing_velocity * diff.velocity.speed;
		const float slider_tick_distance = current_beat_length / speed;
		// Đầu
		push_back(create_spacing_object(
			memory.find(HitObjectSkin[current_direction += slider.rotation][HitObjectType::SLIDER_BEGIN]),
			slider_object, speed, previous));

		const float slider_total_time = static_cast<float>(slider.end_time - slider.time);
		float current_slider_time = 0;
		//int32_t current_curve = 0;
		auto previous_pos = size() - 1;
		// Thân
		while (current_slider_time < slider_total_time)
		{
			/*
			// Tạo slider_object line (curve trong line)
			if (const auto next_slider_time = current_slider_time + slider_tick_distance;
				current_curve < static_cast<int32_t>(slider.curves.size())
				&& next_slider_time > static_cast<float>(slider.curves[current_curve].after))
				// Phát hiện line mới đi qua curve tiếp theo (đã pass trường hợp ko có curve)
			{
				const float time_length_after_curve = next_slider_time - static_cast<float>(slider.curves[current_curve].after);
				//! before curve
				const float time_length_before_curve = slider_tick_distance - time_length_after_curve;
				push_back(create_adjacent_object(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_LINE]),
					translate_width_based_on_direction(speed * time_length_before_curve),
					at(previous_pos), time_length_before_curve / slider_tick_distance));
				current_slider_time += time_length_before_curve;
				previous_pos = size() - 1;
				//! curve
				// Thêm curve (chưa viết code)
				current_direction += slider.curves[current_curve].rotation;
				++current_curve;

				//! after curve
				// (giống khi không bị dính curve, nhưng retain từ cuối)
				push_back(create_adjacent_object(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_LINE]),
					translate_width_based_on_direction(speed * time_length_after_curve), at(previous_pos),
					time_length_after_curve / slider_tick_distance, false));
				previous_pos = size() - 1;
			}
			else
			{
			*/
			// Tạo slider_object line (bình thường)
			const float current_tick_distance =
				Utilities::Math::min_float(slider_tick_distance, slider_total_time - current_slider_time);
			const float current_tick_distance_in_percent = current_tick_distance / slider_tick_distance;
			Object slider_line = create_adjacent_object(
				memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_LINE]),
				translate_width_based_on_direction(speed * current_tick_distance), at(previous_pos),
				current_tick_distance_in_percent);
			push_back(std::move(slider_line));
			previous_pos = size() - 1;
			current_slider_time += current_tick_distance;

			// Tạo slider_object point
			if (current_slider_time < slider_total_time)
				push_back(create_slider_point(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_POINT]), at(previous_pos)));
			// không đặt previous là slider point, vì slider_point có thể có khoảng trống trong src
		}

		// Cuối
		push_back(create_adjacent_object(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_END]),
			{ Config::GameConfig::HitObject::SIZE, Config::GameConfig::HitObject::SIZE }, at(previous_pos)));
	}

	// ::Collection
	Collection::Collection(
		const TextureMemory& memory,
		const Game::Beatmap::HitObjects::HitObjects& hit_objects,
		const Game::Beatmap::Metadata::CalculatedDifficulty& difficulty,
		const Game::Beatmap::TimingPoints::TimingPoints& timing_points)
	{
		auto current_uninherited = timing_points.begin();
		auto current_inherited = timing_points.begin();
		auto next_timing_point = timing_points.begin();

		std::weak_ptr<HitObject> previous;
		for (const auto& hit_object : hit_objects | std::views::values)
		{
			while (next_timing_point != timing_points.end() &&
				hit_object.get_time() >= next_timing_point->first)
			{
				if (next_timing_point->second.beat_length < 0)
					// inherited
					current_inherited = next_timing_point;
				else
					// uninherited
					current_uninherited = next_timing_point;
				++next_timing_point;
			}

			switch (hit_object.get_type())
			{
			case Types::Game::HitObject::HitObjectType::FLOOR:
			{
				const auto current = std::make_shared<Floor>(
					hit_object, memory, difficulty, current_inherited->second.get_velocity(),
					previous.lock().get());
				push_back(current);
				previous = current;
			}
			break;

			case Types::Game::HitObject::HitObjectType::SLIDER:
			{
				const auto current = std::make_shared<Slider>(hit_object, memory, difficulty,
					current_uninherited->second.beat_length, current_inherited->second.get_velocity(),
					previous.lock().get());
				push_back(current);
				previous = current;
			}
			break;
			}
		}
	}
}