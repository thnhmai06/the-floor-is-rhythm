#include "render/playground/hitobject.h" // Header
#include "format/skin.h"
#include "config.h"
#include "template.h"

using SkinFormat::HitObject::HitObjectType, SkinFormat::HitObject::HitObjectSkin;
static auto current_direction = Template::Game::Direction::Direction::RIGHT;

static SDL_FPoint get_size_follow_speed(const float& width)
{
	SDL_FPoint size = { GameConfig::HitObject::SIZE, GameConfig::HitObject::SIZE };
	switch (current_direction)
	{
	case Template::Game::Direction::Direction::RIGHT:
	case Template::Game::Direction::Direction::LEFT:
		size.x = width;
		break;

	case Template::Game::Direction::Direction::UP:
	case Template::Game::Direction::Direction::DOWN:
		size.y = width;
		break;
	}

	return size;
}

//! GameObjects::HitObjects
// ::RenderHitObject
//? static
RenderObjects::RenderObject RenderObjects::Playground::RenderHitObject::create_adjacent_object(
	const Texture& texture,
	const SDL_FPoint& size,
	const RenderObject& previous,
	float src_width_in_percent,
	const bool src_from_beginning)
{
	RenderObject object(texture, Template::Render::RenderOriginType::CENTRE);
	object.config.render_pos = previous.config.render_pos;
	float src_x_in_percent = 0;
	// size
	object.set_scale_fixed(size);
	// pos
	if (!src_from_beginning)
	{
		src_x_in_percent = 1 - src_width_in_percent;
		src_width_in_percent = 1 - src_width_in_percent;
	}
	// Chia 2 vì vị trí đặt vào màn hình là centre của object
	switch (current_direction)
	{
	case Template::Game::Direction::Direction::RIGHT:
		object.src_rect_in_percent.x = src_x_in_percent;
		object.src_rect_in_percent.w = src_width_in_percent;
		object.config.render_pos.x += previous.get_render_size().x / 2 + object.get_render_size().x / 2;
		//object.config.render_pos.y += 10; // for debugging
		break;
	case Template::Game::Direction::Direction::UP:
		object.src_rect_in_percent.y = src_x_in_percent;
		object.src_rect_in_percent.h = src_width_in_percent;
		object.config.render_pos.y -= object.get_render_size().y / 2;
		break;
	case Template::Game::Direction::Direction::LEFT:
		object.src_rect_in_percent.x = src_x_in_percent;
		object.src_rect_in_percent.w = src_width_in_percent;
		object.config.render_pos.x -= object.get_render_size().x / 2;
		break;
	case Template::Game::Direction::Direction::DOWN:
		object.src_rect_in_percent.y = src_x_in_percent;
		object.src_rect_in_percent.h = src_width_in_percent;
		object.config.render_pos.y += previous.get_render_size().y / 2 + object.get_render_size().y / 2;
		break;
	}

	return object;
}
RenderObjects::RenderObject RenderObjects::Playground::RenderHitObject::create_spacing_object(
	const Texture& texture,
	const GameObjects::HitObjects::HitObject& current,
	const float& speed,
	const RenderHitObject* previous)
{
	RenderObject object(texture, Template::Render::RenderOriginType::CENTRE);
	// Pos
	if (!previous || !previous->hit_object)
	{
		current_direction = Template::Game::Direction::Direction::RIGHT + current.get_rotation(); // reset
		object.config.render_pos = 
			SDL_FPoint{
				GameConfig::HitObject::DEFAULT_POS_X,
				GameConfig::HitObject::DEFAULT_POS_Y
			};
	}
	else
	{
		const float time_distance = static_cast<float>(current.get_time() - previous->hit_object->get_end_time());
		object.config.render_pos = previous->back().config.render_pos;
		switch (current_direction)
		{
		case Template::Game::Direction::Direction::RIGHT:
			object.config.render_pos.x += speed * time_distance;
			break;

		case Template::Game::Direction::Direction::UP:
			object.config.render_pos.y -= speed * time_distance;
			break;

		case Template::Game::Direction::Direction::LEFT:
			object.config.render_pos.x -= speed * time_distance;
			break;

		case Template::Game::Direction::Direction::DOWN:
			object.config.render_pos.y += speed * time_distance;
			break;
		}
	}
	object.set_scale_fixed(GameConfig::HitObject::SIZE);

	return object;
}
//? public
RenderObjects::Playground::RenderHitObject::RenderHitObject(
	const Texture& texture,
	const GameObjects::HitObjects::HitObject& current,
	const float& speed,
	const RenderHitObject* previous) : hit_object(&current)
{
	push_back(create_spacing_object(texture, current, speed, previous));
}

// ::RenderFloor
RenderObjects::Playground::RenderFloor::RenderFloor(
	const GameObjects::HitObjects::HitObject& floor_hit_object,
	const TextureMemory& memory,
	const GameObjects::Metadata::CalculatedDifficulty& diff,
	const float& current_timing_velocity,
	const RenderHitObject* previous) :
	RenderHitObject(memory.find(HitObjectSkin[current_direction += floor_hit_object.get_rotation()][HitObjectType::FLOOR]),
		floor_hit_object, current_timing_velocity * diff.velocity.speed, previous)
{
}

// ::RenderSlider
RenderObjects::RenderObject RenderObjects::Playground::RenderSlider::create_slider_point(
	const Texture& texture,
	const RenderObject& previous)
{
	RenderObject current(texture, Template::Render::RenderOriginType::CENTRE);
	current.config.render_pos.x = previous.config.render_pos.x;
	current.config.render_pos.y = previous.config.render_pos.y;
	SDL_FPoint size = { GameConfig::HitObject::SIZE, GameConfig::HitObject::SIZE };
	switch (current_direction)
	{
	case Template::Game::Direction::Direction::RIGHT:
		size.x = GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
		current.config.render_pos.x = previous.get_render_sdl_pos().x + previous.get_render_size().x;
		break;

	case Template::Game::Direction::Direction::LEFT:
		size.x = GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
		current.config.render_pos.x = previous.get_render_sdl_pos().x - size.x;
		break;

	case Template::Game::Direction::Direction::UP:
		size.y = GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
		current.config.render_pos.y = previous.get_render_sdl_pos().y - size.y;
		break;

	case Template::Game::Direction::Direction::DOWN:
		size.y = GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
		current.config.render_pos.y = previous.get_render_sdl_pos().y + previous.get_render_size().y;
		break;
	}
	current.set_scale_fixed(size);
	return current;
}
RenderObjects::Playground::RenderSlider::RenderSlider(
	const GameObjects::HitObjects::HitObject& slider_object,
	const TextureMemory& memory,
	const GameObjects::Metadata::CalculatedDifficulty& diff,
	const float& current_beatlength,
	const float& current_timing_velocity,
	const RenderHitObject* previous) :
	RenderHitObject()
{
	hit_object = &slider_object;
	const auto slider = *std::get_if<GameObjects::HitObjects::Slider>(&slider_object);
	const auto speed = current_timing_velocity * diff.velocity.speed;
	// Đầu
	push_back(create_spacing_object(
		memory.find(HitObjectSkin[current_direction += slider.rotation][HitObjectType::SLIDER_BEGIN]),
		slider_object, speed, previous));

	const float slider_total_time = static_cast<float>(slider.end_time - slider.time);
	float current_slider_time = 0;
	int32_t current_curve = 0;
	auto previous_pos = size() - 1;
	// Thân
	while (current_slider_time < slider_total_time)
	{
		if (const auto next_slider_time = current_slider_time + current_beatlength;
			current_curve < static_cast<int32_t>(slider.curves.size())
			&& next_slider_time > static_cast<float>(slider.curves[current_curve].after))
			// Phát hiện line mới đi qua curve tiếp theo (đã pass trường hợp ko có curve)
		{
			const float time_length_after_curve = next_slider_time - static_cast<float>(slider.curves[current_curve].after);
			//! before curve
			const float time_length_before_curve = current_beatlength - time_length_after_curve;
			push_back(create_adjacent_object(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_LINE]),
				get_size_follow_speed(speed * time_length_before_curve), 
				at(previous_pos), time_length_before_curve / current_beatlength));
			current_slider_time += time_length_before_curve;
			previous_pos = size() - 1;
			//! curve
			//TODO: Thêm curve
			current_direction += slider.curves[current_curve].rotation;
			++current_curve;

			//! after curve
			// (giống khi không bị dính curve, nhưng retain từ cuối)
			push_back(create_adjacent_object(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_LINE]),
				get_size_follow_speed(speed * time_length_after_curve), at(previous_pos),
				time_length_after_curve / current_beatlength, false));
			previous_pos = size() - 1;
		}
		// Tạo slider_object line (bình thường)
		else 
		{
			push_back(create_adjacent_object(
				memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_LINE]),
				get_size_follow_speed(speed * current_beatlength), at(previous_pos)));
			previous_pos = size() - 1;
		}
		current_slider_time += current_beatlength;

		// Tạo slider_object point
		if (current_slider_time < slider_total_time)
			push_back(create_slider_point(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_POINT]), at(previous_pos)));
		// không đặt previous là slider point, vì sliderpoint có thể có khoảng trống trong src
	}

	// Cuối
	push_back(create_adjacent_object(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_END]),
		{GameConfig::HitObject::SIZE, GameConfig::HitObject::SIZE}, at(previous_pos)));
}