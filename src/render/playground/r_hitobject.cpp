#include "render/playground/r_hitobject.h" // Header
#include "rule/skin.h"
#include "rule/config.h"
#include "utilities.h"
#include "template.h"

using SkinFormat::HitObject::HitObjectType, SkinFormat::HitObject::HitObjectSkin;
static auto current_direction = Template::Game::Direction::Direction::RIGHT;

// ::RenderHitObject
RenderObjects::RenderObject RenderObjects::Playground::RenderHitobject::create_object_on_pos(
	const Texture& texture,
	const SDL_FPoint& pos, 
	const SDL_FPoint& size)
{
	RenderObject current(texture, Template::Render::RenderOriginType::CENTRE);
	current.config.render_pos = pos;
	current.set_scale_fixed(size);
	return current;
}
RenderObjects::RenderObject RenderObjects::Playground::RenderHitobject::create_adjacent_object(
	const Texture& texture,
	const float& speed,
	const float& duration,
	const RenderObject* previous,
	float src_width_in_percent,
	const bool src_from_beginning)
{
	RenderObject current(texture, Template::Render::RenderOriginType::CENTRE);
	current.config.render_pos = previous->config.render_pos;
	float src_x_in_percent = 0;
	if (!src_from_beginning) 
	{
		src_x_in_percent = 1 - src_width_in_percent;
		src_width_in_percent = 1 - src_width_in_percent;
	}
	switch (current_direction)
	{
	case Template::Game::Direction::Direction::RIGHT:
		current.src_rect_in_percent.x = src_x_in_percent;
		current.src_rect_in_percent.w = src_width_in_percent;
		current.set_scale_fixed({ speed * duration, GameConfig::HitObject::SIZE_HEIGHT });
		current.config.render_pos.x += previous->get_render_size().x;
		break;
	case Template::Game::Direction::Direction::UP:
		current.src_rect_in_percent.y = src_x_in_percent;
		current.src_rect_in_percent.h = src_width_in_percent;
		current.set_scale_fixed({ GameConfig::HitObject::SIZE_HEIGHT, speed * duration });
		current.config.render_pos.y -= previous->get_render_size().y;
		break;
	case Template::Game::Direction::Direction::LEFT:
		current.src_rect_in_percent.x = src_x_in_percent;
		current.src_rect_in_percent.w = src_width_in_percent;
		current.set_scale_fixed({ speed * duration, GameConfig::HitObject::SIZE_HEIGHT });
		current.config.render_pos.x -= current.get_render_size().x;
		break;
	case Template::Game::Direction::Direction::DOWN:
		current.src_rect_in_percent.y = src_x_in_percent;
		current.src_rect_in_percent.h = src_width_in_percent;
		current.set_scale_fixed({ GameConfig::HitObject::SIZE_HEIGHT, speed * duration });
		current.config.render_pos.y += current.get_render_size().y;
		break;
	}

	return current;
}
RenderObjects::RenderObject RenderObjects::Playground::RenderHitobject::create_spacing_object(
	const Texture& texture,
	const GameObjects::HitObjects::HitObject* current,
	const float& speed,
	const float& duration,
	const RenderHitobject* previous)
{
	RenderObject render_hit_object(texture, Template::Render::RenderOriginType::CENTRE);
	// Pos
	if (!previous || !previous->hit_object)
	{
		current_direction = Template::Game::Direction::Direction::RIGHT + current->rotation; // reset
		render_hit_object.config.render_pos = SDL_FPoint{ GameConfig::HitObject::DEFAULT_POS_X, GameConfig::HitObject::DEFAULT_POS_X };
	}
	else
	{
		const float time_distance = static_cast<float>(current->time - previous->hit_object->end_time);
		render_hit_object.config.render_pos = previous->back().config.render_pos;
		switch (current_direction)
		{
		case Template::Game::Direction::Direction::RIGHT:
			render_hit_object.set_scale_fixed({ speed * duration, GameConfig::HitObject::SIZE_HEIGHT });
			render_hit_object.config.render_pos.x += speed * time_distance;
			break;

		case Template::Game::Direction::Direction::UP:
			render_hit_object.set_scale_fixed({ GameConfig::HitObject::SIZE_HEIGHT, speed * duration });
			render_hit_object.config.render_pos.y -= speed * time_distance;
			break;

		case Template::Game::Direction::Direction::LEFT:
			render_hit_object.set_scale_fixed({ speed * duration, GameConfig::HitObject::SIZE_HEIGHT });
			render_hit_object.config.render_pos.x -= speed * time_distance;
			break;

		case Template::Game::Direction::Direction::DOWN:
			render_hit_object.set_scale_fixed({ GameConfig::HitObject::SIZE_HEIGHT, speed * duration });
			render_hit_object.config.render_pos.y += speed * time_distance;
			break;
		}
	}

	return render_hit_object;
}
RenderObjects::Playground::RenderHitobject::RenderHitobject(
	const Texture& texture,
	const GameObjects::HitObjects::HitObject* current,
	const float& speed,
	const float& duration,
	const RenderHitobject* previous) : hit_object(current)
{
	push_back(create_spacing_object(texture, current, speed, duration, previous));
}

// ::RenderFloor
RenderObjects::Playground::RenderFloor::RenderFloor(
	const GameObjects::HitObjects::Floor* floor,
	const TextureMemory& memory,
	const GameObjects::Metadata::CalculatedDifficulty* diff,
	const float& current_timing_velocity,
	const RenderHitobject* previous) :
	RenderHitobject(memory.find(HitObjectSkin[current_direction += floor->rotation][HitObjectType::FLOOR]),
			floor, current_timing_velocity* diff->velocity.speed, diff->od.bad * 2, previous)
{
}

// ::RenderSlider
RenderObjects::RenderObject RenderObjects::Playground::RenderSlider::create_slider_point(
	const Texture& texture,
	const RenderObject& previous)
{
	SDL_FPoint pos = previous.config.origin_pos; // vị trí centre
	SDL_FPoint size = { GameConfig::HitObject::SIZE_HEIGHT, GameConfig::HitObject::SIZE_HEIGHT };
	switch (current_direction) // Chuyển về end centre
	{
	case Template::Game::Direction::Direction::RIGHT:
		pos.x += previous.get_render_size().x / 2;
		size.x = GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
		break;

	case Template::Game::Direction::Direction::LEFT:
		pos.x -= previous.get_render_size().x / 2;
		size.x = GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
		break;

	case Template::Game::Direction::Direction::UP:
		pos.y -= previous.get_render_size().y / 2;
		size.y = GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
		break;

	case Template::Game::Direction::Direction::DOWN:
		pos.y += previous.get_render_size().y / 2;
		size.y = GameConfig::HitObject::SLIDER_POINT_SIZE_WIDTH;
		break;
	}
	return create_object_on_pos(texture, pos, size);
}
RenderObjects::Playground::RenderSlider::RenderSlider(
	const GameObjects::HitObjects::Slider* slider,
	const TextureMemory& memory,
	const GameObjects::Metadata::CalculatedDifficulty* diff,
	const float& current_beatlength,
	const float& current_timing_velocity,
	const RenderHitobject* previous) :
	RenderHitobject()
{
	hit_object = slider;
	const auto speed = current_timing_velocity * diff->velocity.speed;
	const auto duration = diff->od.bad * 2;
	// Đầu
	push_back(create_spacing_object(
		memory.find(HitObjectSkin[current_direction += slider->rotation][HitObjectType::SLIDER_BEGIN]),
		slider, speed, duration, previous));

	const float slider_total_time = static_cast<float>(slider->end_time - slider->time);
	float current_slider_time = 0;
	int32_t current_curve = 0;
	// Thân
	while (current_slider_time < slider_total_time)
	{
		auto next_slider_time = current_slider_time + current_beatlength;
		if (current_curve < static_cast<int32_t>(slider->curves.size())
			&& next_slider_time > static_cast<float>(slider->curves[current_curve].after))
			// Phát hiện line mới đi qua curve tiếp theo (đã pass trường hợp ko có curve)
		{
			const float time_length_after_curve = next_slider_time - static_cast<float>(slider->curves[current_curve].after);
			// before curve
			const float time_length_before_curve = current_beatlength - time_length_after_curve;
			push_back(create_adjacent_object(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_LINE]), 
				speed, time_length_before_curve, &back(), time_length_before_curve / current_beatlength));
			current_slider_time += time_length_before_curve;

			// curve
			//TODO: Thêm curve
			current_direction += slider->curves[current_curve].rotation;
			++current_curve;

			// after curve
			// (giống khi không bị dính curve, nhưng retain từ cuối)
			push_back(create_adjacent_object(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_LINE]),
				speed, time_length_after_curve, &back(), time_length_after_curve / current_beatlength, false));
		}
		// Tạo slider line (bình thường)
		else push_back(create_adjacent_object(
			memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_LINE]),
			speed, current_beatlength, &back()));
		current_slider_time += current_beatlength;

		// Tạo slider point
		if (current_slider_time < slider_total_time)
			push_back(create_slider_point(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_POINT]), back()));
	}

	// Cuối
	push_back(create_adjacent_object(memory.find(HitObjectSkin[current_direction][HitObjectType::SLIDER_END]),
		speed, duration, &back()));
}