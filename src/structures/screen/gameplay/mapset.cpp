// ReSharper disable CommentTypo
#include "structures/screen/gameplay/mapset.h" // Header
#include <ranges>
#include "format/skin.h"
#include "config.h"
#include "structures/game/mapset/hitobject.h"

namespace Structures::Screen::Gameplay::Mapset
{
	namespace Render
	{
		namespace Components
		{
			//! Floor
			void Floor::add_animation(
				Engine::Events::Action::Buffer& game_buffer,
				const std::pair<const Engine::Events::Timing::Time, const SDL_FPoint>& from,
				const std::pair<const Engine::Events::Timing::Time, const SDL_FPoint>& to)
			{
				using Utilities::Math::FPoint::operator-;

				const auto fade = std::make_shared<Engine::Events::Action::Render::FadeAction>(
					from.first, from.first, Engine::Events::Timing::Easing::Easing::Linear, config, 1, 1);
				const auto move = std::make_shared<Engine::Events::Action::Render::MoveAction>(
					from.first, to.first, Engine::Events::Timing::Easing::Easing::Linear,
					config, Utilities::Math::FPoint::to_float_point(0, 0), from.second - to.second); // vì trừ bên ngoài thành ngược lại

				game_buffer.submit(fade);
				game_buffer.submit(move);
			}
			Floor::Floor(
				const Game::Beatmap::HitObjects::Floor& floor,
				const Memory& memory,
				Engine::Events::Action::Buffer& game_buffer,
				const std::pair<const Engine::Events::Timing::Time, const SDL_FPoint>& from,
				const std::pair<const Engine::Events::Timing::Time, const SDL_FPoint>& to)
				: object(&floor)
			{
				// Object
				const auto texture = memory.find(Format::Skin::Image::HitObject::floor);
				auto r_floor = std::make_shared<Object>(texture, Engine::Render::OriginType::Centre, from.second);
				r_floor->set_render_size(::Config::Game::Render::HitObject::get_size());
				r_floor->config.color = (floor.is_kat) ? ::Config::Game::Render::HitObject::KAT : ::Config::Game::Render::HitObject::DON;

				// Overlay
				auto r_overlay = std::make_shared<Object>(
					memory.find(Format::Skin::Image::HitObject::floor_overlay),
					Engine::Render::OriginType::Centre, from.second);
				r_overlay->set_render_size(::Config::Game::Render::HitObject::get_size());

				data.emplace_back(std::move(r_floor));
				data.emplace_back(std::move(r_overlay));

				config.alpha.percent = 0;
				add_animation(game_buffer, from, to);
			}
		}

		//! Mapset
		const Mapset::RenderScripts& Mapset::get_scripts() const { return render_scripts; }
		Mapset::Mapset(
			const Memory& skin,
			Engine::Events::Action::Buffer& game_actions,
			const Game::Beatmap::Mapset& mapset)
			: timing_points(&mapset.timing_points)
		{
			static constexpr SDL_FPoint centre_pos = { 0, 0 };

			auto timing_point = timing_points->data.begin();
			const auto& velocity_diff = mapset.calculated_difficulty->velocity;
			auto object = mapset.hit_objects.data.begin();

			while (object != mapset.hit_objects.data.end())
			{
				// Cập nhật timing point
				float timing_velocity = 1;
				float beat_length = 60.0f / 180.0f;
				while (timing_point != timing_points->data.end() && timing_point->first < object->first)
				{
					++timing_point;
					if (timing_point == timing_points->data.end()) break;
					if (timing_point->second.inherited)
						timing_velocity = timing_point->second.get_velocity();
					else beat_length = timing_point->second.beat_length;
				}

				// Xác định vị trí, thời điểm
				using Utilities::Math::FPoint::operator/, Utilities::Math::FPoint::operator*;
				const auto distance = Utilities::Math::FPoint::to_float_point(::Config::user_config->graphic.window_size) / 2 * 1.5f; // cho xuất hiện từ trước
				const auto travel_x_time = distance.x / velocity_diff.get_one_pixel_time(timing_velocity, beat_length);
				const auto start_time = object->first - static_cast<Engine::Events::Timing::Time>(std::round(travel_x_time));
				SDL_FPoint start_pos = centre_pos;
				if (!object->second.is_kat)
					start_pos.x -= distance.x;
				else start_pos.x += distance.x;

				auto r_obj = std::make_shared<Components::Floor>(
					object->second, skin, game_actions,
					std::make_pair(start_time, start_pos),
					std::make_pair(object->first, centre_pos));
				data.emplace_back(r_obj);
				render_scripts.emplace(&object->second, r_obj);
				++object;
			}
		}
	}
}
// D3ST1N4T10N U4KN0WN...