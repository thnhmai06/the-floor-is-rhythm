#include "structures/screens/gameplay/playing/mapset.h" // Header
#include <ranges>
#include "format/skin.h"
#include "config.h"
#include "structures/types.h"
#include "utilities.hpp"

namespace Structures::Screens::Gameplay::Playing::Mapset
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
	using Format::Skin::Image::HitObject::HitObjectType, Format::Skin::Image::HitObject::hit_objects_skin;
	// ::Floor
	Floor::Floor(
		const Game::Beatmap::HitObjects::HitObject& floor,
		const TextureMemory& memory,
		const Game::Beatmap::Metadata::CalculatedDifficulty& diff,
		const float& current_timing_velocity,
		const bool no_rotation,
		const HitObject* previous)
	{
		hit_object = &floor;
		if (!no_rotation) current_direction += floor.get_rotation();

		const auto texture = memory.find(
			hit_objects_skin[current_direction][
				HitObjectType::FLOOR]);
		const auto speed = diff.velocity.speed * current_timing_velocity;
		Object current{ texture, Types::Render::OriginType::CENTRE };

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
		const auto texture = memory.find(hit_objects_skin[current_direction][HitObjectType::SLIDER_LINE]);
		const auto previous_render_size = Utilities::Render::get_size_from_rect(previous.get_sdl_dst_rect());

		Object current(texture, Types::Render::OriginType::CENTRE);
		current.config.render_pos = previous.config.render_pos;
		current.set_render_size(size);
		switch (current_direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
			current.src_rect_in_percent.w = src_width_in_percent;
			if (!from_centre_of_previous) current.config.render_pos.x += previous_render_size.x / 2;
			current.config.render_pos.x += size.x / 2;
			//current.config.render_pos.y += 10; // for debugging
			break;
		case Types::Game::Direction::Direction::UP:
			current.src_rect_in_percent.h = src_width_in_percent;
			if (!from_centre_of_previous) current.config.render_pos.y -= previous_render_size.y / 2;
			current.config.render_pos.y -= size.y / 2;
			break;
		case Types::Game::Direction::Direction::LEFT:
			current.src_rect_in_percent.w = src_width_in_percent;
			if (!from_centre_of_previous) current.config.render_pos.x -= previous_render_size.x / 2;
			current.config.render_pos.x -= size.x / 2;
			break;
		case Types::Game::Direction::Direction::DOWN:
			current.src_rect_in_percent.h = src_width_in_percent;
			if (!from_centre_of_previous) current.config.render_pos.y += previous_render_size.y / 2;
			current.config.render_pos.y += size.y / 2;
			break;
		}

		return current;
	}
	Object Slider::create_slider_point(
		const TextureMemory& memory,
		const Object& previous)
	{
		const auto texture = memory.find(hit_objects_skin[current_direction][HitObjectType::SLIDER_POINT]);
		const auto previous_sdl_render_pos = Utilities::Render::get_pos_from_rect(previous.get_sdl_dst_rect());
		const auto previous_render_size = Utilities::Render::get_size_from_rect(previous.get_sdl_dst_rect());

		Object current(texture, Types::Render::OriginType::CENTRE);
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
		current.config.render_pos = previous.config.render_pos;
		switch (current_direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
			current.config.render_pos.x = previous_sdl_render_pos.x + previous_render_size.x;
			break;
		case Types::Game::Direction::Direction::UP:
			current.config.render_pos.y = previous_sdl_render_pos.y;
			break;
		case Types::Game::Direction::Direction::LEFT:
			current.config.render_pos.x = previous_sdl_render_pos.x;
			break;
		case Types::Game::Direction::Direction::DOWN:
			current.config.render_pos.y = previous_sdl_render_pos.y + previous_render_size.y;
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
		const auto texture = memory.find(hit_objects_skin[current_direction][HitObjectType::SLIDER_POINT]);
		const auto speed = diff.velocity.speed * current_timing_velocity;

		Object current(texture, Types::Render::OriginType::CENTRE);
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
		const bool no_rotation,
		const HitObject* previous)
	{
		hit_object = &slider;
		if (!no_rotation) current_direction += slider.get_rotation();

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

	// ::Mapset
	void Mapset::move(const float& distance, const Types::Game::Direction::Direction& direction)
	{
		switch (direction)
		{
		case Types::Game::Direction::Direction::RIGHT:
			offset.x -= distance;
			break;
		case Types::Game::Direction::Direction::LEFT:
			offset.x += distance;
			break;
		case Types::Game::Direction::Direction::UP:
			offset.y += distance;
			break;
		case Types::Game::Direction::Direction::DOWN:
			offset.y -= distance;
			break;
		}
	}
	Mapset::Mapset(const TextureMemory& memory, const Game::Beatmap::Beatmap& beatmap)
	{
		//TODO: Chuyển sang dùng Playing::ActionInfo để vẽ

		const auto& difficulty = beatmap.calculated_difficulty;
		const Game::Beatmap::HitObjects::HitObject* current_hit_object = nullptr;
		float current_timing_point_velocity, current_beat_length;

		std::weak_ptr<Floor> previous_floor;
		std::weak_ptr<Slider> previous_slider;
		auto get_previous = [&previous_floor, &previous_slider]() -> const HitObject*
			{
				if (previous_floor.expired() && previous_slider.expired()) return nullptr;
				if (!previous_floor.expired() && previous_slider.expired()) return previous_floor.lock().get();
				if (previous_floor.expired() && !previous_slider.expired()) return previous_slider.lock().get();
				if (previous_floor.lock()->hit_object->get_end_time() < previous_slider.lock()->hit_object->get_end_time())
					return previous_floor.lock().get();
				return previous_slider.lock().get();
			};
		auto if_floor = [&]()
			{
				bool no_rotation = false;
				// nằm trên slider trước đó thì không xoay
				if (!previous_slider.expired() && Utilities::Math::in_range(
					previous_slider.lock()->hit_object->get_time(),
					previous_slider.lock()->hit_object->get_end_time(),
					current_hit_object->get_time(), false, false))
					no_rotation = true;

				auto current = std::make_shared<Floor>(*current_hit_object, memory, difficulty,
					current_timing_point_velocity, no_rotation, get_previous());
				previous_floor = current;
				data.emplace_back(std::move(current));
			};
		auto if_slider = [&]()
			{
				bool no_rotation = false;
				// nằm trên slider trước đó thì không xoay
				if (!previous_slider.expired() && Utilities::Math::in_range(
					previous_slider.lock()->hit_object->get_time(),
					previous_slider.lock()->hit_object->get_end_time(),
					current_hit_object->get_time(), false, false))
					no_rotation = true;

				auto current = std::make_shared<Slider>(*current_hit_object, memory, difficulty,
					current_beat_length, current_timing_point_velocity, no_rotation, get_previous());
				previous_slider = current;
				data.emplace_back(std::move(current));
			};

		beatmap.for_in_hit_objects(if_floor, if_slider, current_hit_object, current_timing_point_velocity, current_beat_length);
	}
}

// :)
