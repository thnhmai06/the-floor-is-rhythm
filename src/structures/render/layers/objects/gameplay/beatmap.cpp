#include "structures/render/layers/objects/gameplay/beatmap.h" // Header
#include <queue>
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
	static Types::Render::RenderOriginType get_centre_origin_type_based_on_direction(const bool reverse = true)
	{
		switch (current_direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
			return reverse ? Types::Render::RenderOriginType::CENTRE_LEFT : Types::Render::RenderOriginType::CENTRE_RIGHT;
		case Types::Game::Direction::Direction::UP:
			return reverse ? Types::Render::RenderOriginType::BOTTOM_CENTRE : Types::Render::RenderOriginType::TOP_CENTRE;
		case Types::Game::Direction::Direction::LEFT:
			return reverse ? Types::Render::RenderOriginType::CENTRE_RIGHT : Types::Render::RenderOriginType::CENTRE_LEFT;
		case Types::Game::Direction::Direction::DOWN:
			return reverse ? Types::Render::RenderOriginType::TOP_CENTRE : Types::Render::RenderOriginType::BOTTOM_CENTRE;
		}
		return Types::Render::RenderOriginType::CENTRE; // unknown
	}

	//! ::Components
	using namespace Components;
	using Format::SkinFormat::HitObject::HitObjectType, Format::SkinFormat::HitObject::HitObjectSkin;
	// ::Floor
	Floor::Floor(
		const Game::Beatmap::HitObjects::HitObject& floor,
		const TextureMemory& memory,
		const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
		const float& current_timing_velocity,
		const HitObject* previous)
	{
		hit_object = &floor;
		const auto texture = memory.find(HitObjectSkin[current_direction += floor.get_rotation()][HitObjectType::FLOOR]);
		const auto speed = diff.velocity.speed * current_timing_velocity;
		Object current{ texture, Types::Render::RenderOriginType::CENTRE };

		// pos
		if (!previous || !previous->hit_object)
		{
			current_direction = Types::Game::Direction::Direction::RIGHT + floor.get_rotation(); // reset
			current.config.render_pos =
				SDL_FPoint{
					::Config::GameConfig::HitObject::DEFAULT_POS_X,
					::Config::GameConfig::HitObject::DEFAULT_POS_Y
			};
		}
		else
		{
			const auto previous_hit_object = previous->hit_object;
			const float time_distance = static_cast<float>(floor.get_time() - previous_hit_object->get_end_time());

			current.config.render_pos = previous->data.back().config.render_pos;
			switch (current_direction)
			{
			case Types::Game::Direction::Direction::RIGHT:
				current.config.render_pos.x += speed * time_distance;
				break;

			case Types::Game::Direction::Direction::UP:
				current.config.render_pos.y -= speed * time_distance;
				break;

			case Types::Game::Direction::Direction::LEFT:
				current.config.render_pos.x -= speed * time_distance;
				break;

			case Types::Game::Direction::Direction::DOWN:
				current.config.render_pos.y += speed * time_distance;
				break;
			}
		}

		// size
		current.set_render_size(::Config::GameConfig::HitObject::SIZE);

		data.push_back(std::move(current));
	}

	// ::Slider
	//? Static
	Object Slider::create_slider_line(
		const TextureMemory& memory,
		const SDL_FPoint& size,
		const Object& previous,
		const float src_width_in_percent,
		const bool from_centre_of_previous)
	{
		const auto texture = memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_LINE]);

		Object object(texture, Types::Render::RenderOriginType::CENTRE);
		object.config.render_pos = previous.config.render_pos;
		object.set_render_size(size);
		switch (current_direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
			object.src_rect_in_percent.w = src_width_in_percent;
			if (!from_centre_of_previous) object.config.render_pos.x += previous.get_render_size().x / 2;
			object.config.render_pos.x += object.get_render_size().x / 2;
			//object.config.render_pos.y += 10; // for debugging
			break;
		case Types::Game::Direction::Direction::UP:
			object.src_rect_in_percent.h = src_width_in_percent;
			if (!from_centre_of_previous) object.config.render_pos.y -= previous.get_render_size().y / 2;
			object.config.render_pos.y -= object.get_render_size().y / 2;
			break;
		case Types::Game::Direction::Direction::LEFT:
			object.src_rect_in_percent.w = src_width_in_percent;
			if (!from_centre_of_previous) object.config.render_pos.x -= previous.get_render_size().x / 2;
			object.config.render_pos.x -= object.get_render_size().x / 2;
			break;
		case Types::Game::Direction::Direction::DOWN:
			object.src_rect_in_percent.h = src_width_in_percent;
			if (!from_centre_of_previous) object.config.render_pos.y += previous.get_render_size().y / 2;
			object.config.render_pos.y += object.get_render_size().y / 2;
			break;
		}

		return object;
	}
	Object Slider::create_slider_point(
		const TextureMemory& memory,
		const Object& previous)
	{
		const auto texture = memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_POINT]);
		Object current(texture, Types::Render::RenderOriginType::CENTRE);

		// size
		SDL_FPoint size = { Config::GameConfig::HitObject::SIZE, Config::GameConfig::HitObject::SIZE };
		switch (current_direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
		case Types::Game::Direction::Direction::LEFT:
			size.x = Config::GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;

			break;

		case Types::Game::Direction::Direction::UP:
		case Types::Game::Direction::Direction::DOWN:
			size.y = Config::GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
			break;
		}
		current.set_render_size(size);

		// pos
		switch (current_direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
			current.config.render_pos.x = previous.get_sdl_render_pos().x + previous.get_render_size().x;
			break;
		case Types::Game::Direction::Direction::UP:
			current.config.render_pos.y = previous.get_sdl_render_pos().y;
			break;
		case Types::Game::Direction::Direction::LEFT:
			current.config.render_pos.x = previous.get_sdl_render_pos().x;
			break;
		case Types::Game::Direction::Direction::DOWN:
			current.config.render_pos.y = previous.get_sdl_render_pos().y + previous.get_render_size().y;
			break;
		}

		return current;
	}
	Object Slider::create_slider_begin(
		const Game::Beatmap::HitObjects::HitObject& slider,
		const TextureMemory& memory,
		const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
		const float& current_timing_velocity,
		const HitObject* previous)
	{
		const auto texture = memory.find(HitObjectSkin[current_direction += slider.get_rotation()][HitObjectType::SLIDER_POINT]);
		const auto speed = diff.velocity.speed * current_timing_velocity;
		Object current(texture, Types::Render::RenderOriginType::CENTRE);

		// pos (giống Floor)
		if (!previous || !previous->hit_object)
		{
			current_direction = Types::Game::Direction::Direction::RIGHT + slider.get_rotation(); // reset
			current.config.render_pos =
				SDL_FPoint{
					::Config::GameConfig::HitObject::DEFAULT_POS_X,
					::Config::GameConfig::HitObject::DEFAULT_POS_Y
			};
		}
		else
		{
			const auto previous_hit_object = previous->hit_object;
			const float time_distance = static_cast<float>(slider.get_time() - previous_hit_object->get_end_time());

			// pos
			current.config.render_pos = previous->data.back().config.render_pos;
			switch (current_direction)
			{
			case Types::Game::Direction::Direction::RIGHT:
				current.config.render_pos.x += speed * time_distance;
				break;

			case Types::Game::Direction::Direction::UP:
				current.config.render_pos.y -= speed * time_distance;
				break;

			case Types::Game::Direction::Direction::LEFT:
				current.config.render_pos.x -= speed * time_distance;
				break;

			case Types::Game::Direction::Direction::DOWN:
				current.config.render_pos.y += speed * time_distance;
				break;
			}
		}

		// size (giống Slider Point)
		SDL_FPoint size = { Config::GameConfig::HitObject::SIZE, Config::GameConfig::HitObject::SIZE };
		switch (current_direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
		case Types::Game::Direction::Direction::LEFT:
			size.x = Config::GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
			break;

		case Types::Game::Direction::Direction::UP:
		case Types::Game::Direction::Direction::DOWN:
			size.y = Config::GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
			break;
		}
		current.set_render_size(size);

		return current;
	}
	//? Public
	Slider::Slider(
		const Game::Beatmap::HitObjects::HitObject& slider,
		const TextureMemory& memory,
		const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
		const float& current_beat_length,
		const float& current_timing_velocity,
		const HitObject* previous)
	{
		hit_object = &slider;

		// slider begin
		data.push_back(create_slider_begin(slider, memory, diff, current_timing_velocity, previous));
		bool is_first_slider_line = true;

		const auto speed = current_timing_velocity * diff.velocity.speed;
		const float slider_tick_distance = current_beat_length / speed;
		const float slider_total_time = static_cast<float>(hit_object->get_end_time() - hit_object->get_time());
		float current_slider_time = 0;

		while (current_slider_time < slider_total_time)
		{
			// slider line
			const float current_tick_distance = std::min(slider_tick_distance, slider_total_time - current_slider_time);
			const float current_tick_distance_in_percent = current_tick_distance / slider_tick_distance;
			Object slider_line;
			if (is_first_slider_line)
			{
				slider_line = create_slider_line(
					memory, translate_width_based_on_direction(speed * current_tick_distance),
					data.back(),
					current_tick_distance_in_percent, true);
				is_first_slider_line = false;
			}
			else {
				slider_line = create_slider_line(
					memory, translate_width_based_on_direction(speed * current_tick_distance),
					*std::prev(data.end(), 2), // slider line gần nhất
					current_tick_distance_in_percent);
			}
			data.push_back(std::move(slider_line));
			current_slider_time += current_tick_distance;

			// slider_point
			if (current_slider_time < slider_total_time)
				data.push_back(create_slider_point(memory, data.back()));
		}

		// slider end
		data.push_back(create_slider_point(memory, data.back()));
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
		std::queue<const Game::Beatmap::HitObjects::HitObject*> floor, slider;

		const HitObject* previous = nullptr;
		for (const auto& hit_object : hit_objects | std::views::values)
		{
			if (hit_object.get_type() == Types::Game::HitObject::HitObjectType::FLOOR)
				floor.push(&hit_object);
			else if (hit_object.get_type() == Types::Game::HitObject::HitObjectType::SLIDER)
				slider.push(&hit_object);
		}

		while (!floor.empty() && !slider.empty())
		{
			const Game::Beatmap::HitObjects::HitObject* hit_object;
			if (slider.empty() || floor.front()->get_time() < slider.front()->get_time())
			{
				hit_object = floor.front();
				floor.pop();
			}
			else 
			{
				hit_object = slider.front();
				slider.pop();
			}

			// update timing point
			while (next_timing_point != timing_points.end() &&
				hit_object->get_time() >= next_timing_point->first)
			{
				if (next_timing_point->second.beat_length < 0)
					// inherited
					current_inherited = next_timing_point;
				else
					// uninherited
					current_uninherited = next_timing_point;
				++next_timing_point;
			}

			// tạo render obj
			switch (hit_object->get_type())
			{
			case Types::Game::HitObject::HitObjectType::FLOOR:
			{
				auto current = std::make_unique<Floor>(*hit_object, memory, difficulty,
					current_inherited->second.get_velocity(), previous);
				data.emplace_back(std::move(current));
				previous = dynamic_cast<HitObject*>(std::get_if<PolyObjectUnique>(&data.back())->get());
			}
			break;

			case Types::Game::HitObject::HitObjectType::SLIDER:
			{
				auto current = std::make_unique<Slider>(*hit_object, memory, difficulty,
					current_uninherited->second.beat_length, current_inherited->second.get_velocity(), previous);
				data.emplace_back(std::move(current));
				previous = dynamic_cast<HitObject*>(std::get_if<PolyObjectUnique>(&data.back())->get());
			}
			break;
			}
		}
	}
}