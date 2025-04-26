#include "structures/screens/gameplay/playing/mapset.h" // Header
#include <ranges>
#include "structures/render/objects/percent.h"
#include "format/skin.h"
#include "config.h"
#include "utilities.hpp"

namespace Structures::Screens::Gameplay::Playing::Mapset
{
	namespace Components
	{
		//! HitObject
		HitObject::HitObject(const Game::Beatmap::HitObjects::Floor* hit_object)
			: PolyObject(), hit_object(hit_object)
		{
		}
		HitObject::HitObject(const std::weak_ptr<const Game::Beatmap::HitObjects::Floor>& hit_object)
			: HitObject(hit_object.lock().get())
		{
		}

		//! Floor
		Floor::Floor(
			const Game::Beatmap::HitObjects::Floor* floor,
			const TextureMemory& memory,
			const SDL_FPoint& centre_pos,
			const SDL_FPoint& size)
			: HitObject(floor)
		{
			const auto texture = memory.find(Format::Skin::Image::HitObject::floor);
			Object current{ texture, Types::Render::OriginType::CENTRE, centre_pos };
			current.set_render_size(size);
			data.push_back(std::move(current));
		}

		// ::Slider
		float Slider::add_slider_line(
			const TextureMemory& memory,
			const SDL_FPoint& centre_width_edge_pos,
			const float& full_width,
			const float& percent)
		{
			const auto texture = memory.find(Format::Skin::Image::HitObject::slider_line);
			const SDL_FPoint full_size = { full_width, Config::GameConfig::HitObject::HIT_OBJECT_SIZE };
			StaticPercentObject current{
				texture, full_size, true, false,
				to_right ? Types::Render::OriginType::CENTRE_LEFT : Types::Render::OriginType::CENTRE_RIGHT,
				centre_width_edge_pos };
			current.percent = percent;
			const uint32_t current_index = static_cast<uint32_t>(data.size());
			data.push_back(std::move(current));
			slider_line_index.push_back(current_index);
			return full_width * percent;
		}
		void Slider::add_slider_point(
			const TextureMemory& memory,
			const SDL_FPoint& centre_pos,
			const SDL_FPoint& size)
		{
			const auto texture = memory.find(Format::Skin::Image::HitObject::slider_point);
			Object current(texture, Types::Render::OriginType::CENTRE, centre_pos);
			current.set_render_size(size);
			const uint32_t current_index = static_cast<uint32_t>(data.size());
			data.push_back(std::move(current));
			slider_point_index.push_back(current_index);
		}
		void Slider::add_slider_begin(
			const TextureMemory& memory,
			const SDL_FPoint& centre_pos,
			const SDL_FPoint& size)
		{
			const auto texture = memory.find(Format::Skin::Image::HitObject::floor);
			Object current(texture, Types::Render::OriginType::CENTRE, centre_pos);
			current.set_render_size(size);
			const uint32_t current_index = static_cast<uint32_t>(data.size());
			data.push_back(std::move(current));
			begin_index = current_index;
		}
		void Slider::add_slider_end(
			const TextureMemory& memory,
			const SDL_FPoint& centre_pos,
			const SDL_FPoint& size)
		{
			const auto texture = memory.find(Format::Skin::Image::HitObject::slider_end);
			Object current(texture, Types::Render::OriginType::CENTRE, centre_pos);
			current.set_render_size(size);
			const uint32_t current_index = static_cast<uint32_t>(data.size());
			data.push_back(std::move(current));
			end_index = current_index;
		}
		void Slider::add_slider_reverse_arrow(
			const TextureMemory& memory,
			const SDL_FPoint& size,
			const bool back_to_left)
		{
			const auto texture = memory.find(Format::Skin::Image::HitObject::slider_reverse);
			Object current(texture, Types::Render::OriginType::CENTRE, (back_to_left) ? right_pos : left_pos);
			current.set_render_size(size);
			data.push_back(std::move(current));
		}


		Slider::Slider(
			const Game::Beatmap::HitObjects::Slider* slider,
			const TextureMemory& memory,
			const float& diff_pixel_speed,
			const float& current_beat_length,
			const float& current_timing_velocity,
			const SDL_FPoint& centre_pos,
			const bool to_right)
			: HitObject(slider), to_right(to_right)
		{
			constexpr SDL_FPoint hit_object_size = { Config::GameConfig::HitObject::HIT_OBJECT_SIZE, Config::GameConfig::HitObject::HIT_OBJECT_SIZE };
			constexpr SDL_FPoint slider_point_size = { Config::GameConfig::HitObject::SLIDER_POINT_SIZE, Config::GameConfig::HitObject::SLIDER_POINT_SIZE };

			//! Line, Point
			// Độ dài 1 đoạn slider là 1 beatlength
			const auto current_pixel_speed = diff_pixel_speed * current_timing_velocity;
			auto slider_single_time_left = static_cast<float>(slider->end_time - slider->time) / static_cast<float>(slider->slides);
			auto current_line_pos = centre_pos;
			while (slider_single_time_left > current_beat_length)
			{
				const auto tick_distance = add_slider_line(memory, current_line_pos, current_beat_length * current_pixel_speed, 1.0f);
				if (to_right) current_line_pos.x += tick_distance; else current_line_pos.x -= tick_distance;
				add_slider_point(memory, current_line_pos, slider_point_size);
				slider_single_time_left -= current_beat_length;
			}
			if (slider_single_time_left > 0 && !Utilities::Math::is_equal(slider_single_time_left, 0))
			{
				const auto tick_distance =
					add_slider_line(memory, current_line_pos, current_beat_length * current_pixel_speed, slider_single_time_left / current_beat_length);
				if (to_right) current_line_pos.x += tick_distance; else current_line_pos.x -= tick_distance;
				slider_single_time_left = 0;
			}

			left_pos = centre_pos;
			right_pos = current_line_pos;

			//! Begin, End
			const auto begin_pos = (to_right) ? left_pos : right_pos;
			const auto end_pos = (!to_right) ? left_pos : right_pos;
			add_slider_begin(memory, begin_pos, hit_object_size);
			add_slider_end(memory, end_pos, hit_object_size);

			//! Reverse Arrow
			if (slider->slides >= 2)
			{
				const uint32_t current_index = static_cast<uint32_t>(data.size());
				add_slider_reverse_arrow(memory, hit_object_size, to_right);
				slider_reverse_arrow_index.front() = current_index;
			}
			if (slider->slides >= 3)
			{
				const uint32_t current_index = static_cast<uint32_t>(data.size());
				add_slider_reverse_arrow(memory, hit_object_size, !to_right);
				slider_reverse_arrow_index.back() = current_index;
			}
		}
	}

	Mapset::Mapset(const TextureMemory& memory, const Game::Beatmap::Mapset& beatmap)
	{
		const auto diff_pixel_speed = beatmap.calculated_difficulty->velocity.get_pixel_speed();
		auto timing_points = beatmap.timing_points.to_queue();
		float current_timing_velocity = 1;
		float current_beat_length;

		SDL_FPoint current_pos = { 0, 0 };
		float current_time = 0;
		bool to_right = true;
		for (const auto& obj : beatmap.hit_objects.data | std::views::values)
		{
			//! Cập nhật Timing Point và vị trí object mới
			while (!timing_points.empty() && timing_points.front()->time <= obj->time)
			{
				const auto lower_tm_point = Utilities::Container::get_front_and_pop(timing_points);
				if (lower_tm_point->inherited) current_timing_velocity = lower_tm_point->get_velocity();
				else current_beat_length = lower_tm_point->beat_length;

				//? Tính khoảng cách những đoạn Timing Point trước
				// chắc chắn current_time sẽ phải nhỏ hơn hoặc bằng std::max(int64_t(0), lower_tm_point->time)) - tính cả trường hợp tm_point âm
				const auto distance =
					((current_time - static_cast<float>(std::max(static_cast<int64_t>(0), lower_tm_point->time))) / current_beat_length) * diff_pixel_speed * current_timing_velocity;
				if (to_right) current_pos.x += distance;
				else current_pos.x -= distance;
				current_time = static_cast<float>(lower_tm_point->time);
			}
			//? Tính khoảng cách từ current_time đến obj->time
			if (const auto distance = ((static_cast<float>(obj->time) - current_time) / current_beat_length) * diff_pixel_speed * current_timing_velocity;
				to_right) current_pos.x += distance;
			else current_pos.x -= distance;
			//current_time = obj->time;

			//! Xây dựng Object (Render)
			switch (obj->type_data.type)
			{
			case Types::Game::HitObject::HitObjectType::Floor:
			{
				const auto it = obj.get();
				constexpr SDL_FPoint hit_object_size = { Config::GameConfig::HitObject::HIT_OBJECT_SIZE, Config::GameConfig::HitObject::HIT_OBJECT_SIZE };
				auto result = std::make_shared<Components::Floor>(it, memory, current_pos, hit_object_size);
				current_pos = result->data.front().config.render_pos;
				data.emplace_back(std::move(result));
			}
			break;

			case Types::Game::HitObject::HitObjectType::Slider:
			{
				const auto it = dynamic_cast<Game::Beatmap::HitObjects::Slider*>(obj.get());
				auto result = std::make_shared<Components::Slider>(it, memory, diff_pixel_speed, current_beat_length,
					current_timing_velocity, current_pos, to_right);
				current_pos = result->data.front().config.render_pos;
				to_right = (it->slides & 1) == 1 ? result->to_right : !result->to_right;
				data.emplace_back(std::move(result));
			}
			break;
			}
			current_time = static_cast<float>(obj->end_time);
		}
	}
}
// :)
// D3ST1N4T10N U4KN0WN...
