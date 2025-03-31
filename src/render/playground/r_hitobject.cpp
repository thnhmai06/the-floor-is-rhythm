#include "render/playground/r_hitobject.h" // Header
#include "rule/skin.h"
#include "rule/config.h"
#include "utilities.h"
#include "template.h"

static auto current_direction = Template::Game::Direction::Direction::RIGHT;

// ::RenderHitObject
RenderObjects::RenderObject RenderObjects::Playground::RenderHitobject::create_adjacent_object(
	const std::string* skin_name, 
	const TextureMemory* memory, 
	const SDL_FPoint& spacing, 
	const float& speed, 
	const float& duration, 
	const RenderObject* previous)
{
	RenderObject render_object(skin_name, memory, Template::Render::RenderOriginType::CENTRE);
	// Pos
	if (!previous)
	{
		current_direction = Template::Game::Direction::Direction::RIGHT; // reset
		render_object.config.render_pos = ImmutableConfig::HitObject::DEFAULT_POS;
	}
	else
	{
		render_object.config.render_pos = previous->config.render_pos;
		switch (current_direction)
		{
		case Template::Game::Direction::Direction::RIGHT:
		default:
			render_object.config.render_pos.x += speed * spacing.x;
			break;
		case Template::Game::Direction::Direction::UP:
			render_object.config.render_pos.y -= speed * spacing.y;
			break;
		case Template::Game::Direction::Direction::LEFT:
			render_object.config.render_pos.x -= speed * spacing.x;
			break;
		case Template::Game::Direction::Direction::DOWN:
			render_object.config.render_pos.y += speed * spacing.y;
			break;
		}
	}
}
RenderObjects::RenderObject RenderObjects::Playground::RenderHitobject::create_hit_object(
	const std::string* skin_name, 
	const TextureMemory* memory, 
	const HitObject::HitObject* current,
	const float& speed, 
	const float& duration, 
	const RenderHitobject* previous)
{
	RenderObject render_hit_object(skin_name, memory, Template::Render::RenderOriginType::CENTRE);
	// Pos
	if (!previous || !previous->hit_object)
	{
		current_direction = Template::Game::Direction::Direction::RIGHT + current->rotation; // reset
		render_hit_object.config.render_pos = ImmutableConfig::HitObject::DEFAULT_POS;
	}
	else
	{
		const float time_distance = static_cast<float>(current->time - previous->hit_object->end_time);
		render_hit_object.config.render_pos = previous->back().config.render_pos;
		switch (current_direction)
		{
		case Template::Game::Direction::Direction::RIGHT:
		default:
			render_hit_object.config.render_pos.x += speed * time_distance;
			break;

		case Template::Game::Direction::Direction::UP:
			render_hit_object.config.render_pos.y -= speed * time_distance;
			break;

		case Template::Game::Direction::Direction::LEFT:
			render_hit_object.config.render_pos.x -= speed * time_distance;
			break;

		case Template::Game::Direction::Direction::DOWN:
			render_hit_object.config.render_pos.y += speed * time_distance;
			break;
		}
	}
	// Size
	render_hit_object.config.set_scale_fixed(
		{ speed * duration, ImmutableConfig::HitObject::SIZE_HEIGHT },
		memory->get_texture_size(*skin_name)
	);
	return render_hit_object;
}
RenderObjects::Playground::RenderHitobject::RenderHitobject(
	const std::string* skin_name,
	const TextureMemory* memory,
	const HitObject::HitObject* current,
	const float& speed,
	const float& duration,
	const RenderHitobject* previous) : hit_object(current)
{
	push_back(create_hit_object(skin_name, memory, current, speed, duration, previous));
}

// ::RenderFloor
RenderObjects::Playground::RenderFloor::RenderFloor(
	const HitObject::Floor* floor,
	const TextureMemory* memory,
	const Metadata::CalculatedDifficulty* diff,
	const float& current_timing_velocity,
	const RenderHitobject* previous) :
	RenderHitobject(&SkinFormat::HitObject.find(current_direction += floor->rotation)->second.FLOOR, memory,
		floor, current_timing_velocity * diff->velocity.speed, diff->od.bad * 2, previous) {
}

// ::RenderSlider
RenderObjects::Playground::RenderSlider::RenderSlider(
	const HitObject::Slider* slider,
	const TextureMemory* memory,
	const Metadata::CalculatedDifficulty* diff,
	const float& current_beatlength,
	const float& current_timing_velocity,
	const RenderHitobject* previous) :
	RenderHitobject()
{
	hit_object = slider;
	const auto slider_skin = &SkinFormat::HitObject.find(current_direction += slider->rotation)->second.SLIDER;
	// Đầu
	push_back(create_hit_object(&slider_skin->BEGIN, memory, slider,
		current_timing_velocity * diff->velocity.speed, diff->od.bad * 2, previous));
	// Thân

}

//!::RenderSlider::SliderComponents
RenderObjects::RenderObject RenderObjects::Playground::RenderSlider::SliderComponents::create_slider_line(
	const SDL_FRect& previous_dst,
	const float& src_retain_percent,
	const bool retain_from_beginning,
	const float& dst_width,
	const Direction::Direction& current_direction
) const
{
	RenderObject slider_line_part;
	slider_line_part.target_texture_name = &SkinFormat::HitObject::Suffic_Slider::LINE;
	slider_line_part.config.origin_dst = SDL_FRect();
	if (retain_from_beginning)
		slider_line_part.config.src_percent = SDL_FRect{ 0, 0, src_retain_percent, 1 };
	else
		slider_line_part.config.src_percent = SDL_FRect{ 1 - src_retain_percent, 0, src_retain_percent, 1 };
	// Position
	if (empty())
	{
		slider_line_part.config.origin_dst->x = previous_dst.x;
		slider_line_part.config.origin_dst->y = previous_dst.y;
	}
	else
	{
		switch (current_direction)
		{
		case Direction::Direction::RIGHT:
			slider_line_part.config.origin_dst->x = previous_dst.x + previous_dst.w;
			slider_line_part.config.origin_dst->y = previous_dst.y;
			break;
		case Direction::Direction::LEFT:
			slider_line_part.config.origin_dst->x = previous_dst.x - previous_dst.w;
			slider_line_part.config.origin_dst->y = previous_dst.y;
			break;
		case Direction::Direction::UP:
			slider_line_part.config.origin_dst->x = previous_dst.x;
			slider_line_part.config.origin_dst->y = previous_dst.y + previous_dst.w;
			break;
		case Direction::Direction::DOWN:
			slider_line_part.config.origin_dst->x = previous_dst.x;
			slider_line_part.config.origin_dst->y = previous_dst.y - previous_dst.w;
			break;
		}
	}
	// Size
	slider_line_part.config.origin_dst->w = dst_width;
	slider_line_part.config.origin_dst->h = ImmutableConfig::HitObject::SIZE_HEIGHT;
	// Rotate ảnh theo đúng chiều (rotate tâm ở trung điểm cạnh bên trái)
	slider_line_part.config.rotation = Texture::TextureConfig::RotationConfig(current_direction);
	slider_line_part.config.rotation->center = SDL_FPoint{ 0, slider_line_part.config.origin_dst->h / 2 };

	return slider_line_part;
}
RenderObjects::RenderObject RenderObjects::Playground::RenderSlider::SliderComponents::create_slider_point(
	const Direction::Direction& current_direction) const
{
	RenderObject slider_point;
	slider_point.target_texture_name = &SkinFormat::HitObject::Suffic_Slider::POINT;
	slider_point.config.origin_dst = SDL_FRect{
		back().config.origin_dst->x + back().config.origin_dst->w,
		back().config.origin_dst->y,
		ImmutableConfig::HitObject::SLIDER_POINT_SIZE_WIDTH,
		ImmutableConfig::HitObject::SIZE_HEIGHT
	};
	// Rotate ảnh theo đúng chiều (rotate tâm ở trung tâm)
	slider_point.config.rotation = Texture::TextureConfig::RotationConfig(current_direction);
	return slider_point;
}
RenderObjects::RenderObject RenderObjects::Playground::RenderSlider::SliderComponents::create_slider_curve(
	const Direction::Direction& current_direction,
	const Direction::Rotation& rotation) const
{
	RenderObject slider_curve;
	slider_curve.target_texture_name = &SkinFormat::HitObject::Suffic_Slider::CURVE;

}
RenderObjects::Playground::RenderSlider::SliderComponents::SliderComponents(
	const HitObject::Slider* slider,
	const SDL_FRect* start_dst,
	const float& current_beatlength,
	const float& velocity)
{
	const auto time_length = slider->end_time - slider->time;
	auto current_direction = Direction::Direction::RIGHT;
	float current_padding_time = 0.0f;
	int32_t current_curve = 0;

	while (current_padding_time < static_cast<float>(time_length))
	{
		auto previous_dst = (empty() ? *start_dst : back().config.origin_dst.value());
		const float next_padding_time_if_no_crop = current_padding_time + current_beatlength;

		//! SliderLine - one beat
		if (current_curve < static_cast<int32_t>(slider->curves.size())
			&& next_padding_time_if_no_crop > static_cast<float>(slider->curves[current_curve].padding_time))
			// Phát hiện slider line mới đi qua curve tiếp theo (đã pass trường hợp ko có curve)
		{
			const float excess_length_as_curve =
				next_padding_time_if_no_crop - static_cast<float>(slider->curves[current_curve].padding_time);

			//? ================ before curve ================
			const float slider_line_length_before_curve = current_beatlength - excess_length_as_curve;
			const float src_retain_before_percent = slider_line_length_before_curve / current_beatlength;
			push_back(create_slider_line(previous_dst, src_retain_before_percent, true,
				velocity * current_beatlength * src_retain_before_percent, current_direction));
			current_padding_time += slider_line_length_before_curve;
			//? ==============================================

			//? ================ curve ================
			push_back(create_slider_curve(current_direction,
				slider->curves[current_curve].rotation));
			current_direction += slider->curves[current_curve].rotation;
			++current_curve;
			//? =======================================

			//? ================ after curve ================
			//(giống khi không bị dính curve, nhưng retain từ cuối)
			previous_dst = back().config.origin_dst.value(); // update
			const float excess_time_as_endpoint = Utilities::Math::max_float(
				next_padding_time_if_no_crop - static_cast<float>(time_length),
				0.0f
			);
			const float slider_line_length_after_curve = current_beatlength - slider_line_length_before_curve - excess_time_as_endpoint;
			const float src_retain_after_percent = slider_line_length_after_curve / current_beatlength;
			push_back(create_slider_line(previous_dst, src_retain_after_percent, false,
				velocity * current_beatlength * src_retain_after_percent, current_direction));
			current_padding_time += slider_line_length_after_curve;
			//? ==============================================
		}
		else
		{
			const float excess_time_as_endpoint = Utilities::Math::max_float(
				next_padding_time_if_no_crop - static_cast<float>(time_length),
				0.0f
			);
			const float slider_line_length = current_beatlength - excess_time_as_endpoint;
			const float src_retain_percent = slider_line_length / current_beatlength;
			push_back(create_slider_line(previous_dst, src_retain_percent, true,
				velocity * current_beatlength * src_retain_percent, current_direction));
			current_padding_time += slider_line_length;
		}

		//! SliderPoint
		if (current_padding_time < static_cast<float>(time_length)) // Vẫn chưa đến cuối slider (nếu là cuối thì không tạo)
			push_back(create_slider_point(current_direction));

	}
}
