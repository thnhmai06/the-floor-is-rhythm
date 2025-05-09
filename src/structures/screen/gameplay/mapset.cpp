// ReSharper disable CommentTypo
#include "structures/screen/gameplay/mapset.h" // Header
#include <ranges>
#include "format/skin.h"
#include "config.h"
#include "utilities.hpp"
#include "logging/logger.h"
#include "structures/game/mapset/hitobject.h"

namespace Structures::Screen::Gameplay::Mapset
{
	namespace Render
	{
		namespace Components
		{
			//! Floor
			Floor::Floor(
				const Game::Beatmap::HitObjects::Floor* floor,
				const Memory& memory,
				const SDL_FPoint& centre_pos)
				: object(floor)
			{
				// Object
				const auto texture = memory.find(Format::Skin::Image::HitObject::floor);
				auto r_floor = std::make_shared<Object>(texture, Types::Render::OriginType::Centre, centre_pos);
				r_floor->set_render_size(Config::GameConfig::Render::HitObject::SIZE);
				r_floor->config.color = (floor->is_kat) ? Config::GameConfig::Render::HitObject::KAT : Config::GameConfig::Render::HitObject::DON;

				// Overlay
				auto r_overlay = std::make_shared<Object>(
					memory.find(Format::Skin::Image::HitObject::floor_overlay),
					Types::Render::OriginType::Centre, centre_pos);
				r_overlay->set_render_size(Config::GameConfig::Render::HitObject::SIZE);

				data.emplace_back(std::move(r_floor));
				data.emplace_back(std::move(r_overlay));
			}
		}


		//! Mapset
		void Mapset::on_before_render()
		{
			using Utilities::Math::FPoint::operator*;
			using Utilities::Math::FPoint::operator-;
			right->offset = -offset * 2; // trừ bù
		}
		const Mapset::RenderScripts* Mapset::get_render_scripts() const
		{
			return &render_script;
		}
		void Mapset::set_render_range(const int64_t& current_time)
		{
			constexpr int64_t render_time_range = 3000; // 3s, tạm thời là vậy
			const auto begin_time = current_time;
			const auto end_time = current_time + render_time_range;

			if (render_script.empty()) return;

			auto begin_item = render_script.lower_bound(begin_time);
			auto end_item = render_script.upper_bound(end_time);
			if (end_item == render_script.end()) end_item = Utilities::Container::get_last_element_iterator(render_script);
			else if (end_item != render_script.begin()) --end_item;

			left->render_range.clear();
			right->render_range.clear();

			if (begin_item == render_script.end()) return;
			const auto& begin_script = begin_item->second;
			const auto& end_script = end_item->second;

			left->render_range.emplace_back(begin_script.left_index, end_script.left_index);
			right->render_range.emplace_back(begin_script.right_index, end_script.right_index);
		}
		void Mapset::set_current_pos(const int64_t& current_time)
		{
			offset.x = timing_points->get_object_pos(static_cast<float>(current_time));
		}
		Mapset::Mapset(const Memory& memory, const Game::Beatmap::Mapset& mapset)
			: timing_points(&mapset.timing_points)
		{
			left = std::make_shared<Collection>();
			data.emplace_back(left);
			right = std::make_shared<Collection>();
			data.emplace_back(right);

			for (const auto& obj : mapset.hit_objects.data | std::views::values)
			{
				SDL_FPoint pos = { timing_points->get_object_pos(obj.time), 0 };
				if (!obj.is_kat) pos.x *= -1;
				auto r_obj = std::make_shared<Components::Floor>(&obj, memory, pos);

				if (obj.is_kat)
				{
					const auto left_index = left->data.empty() ? 0 : left->data.size() - 1;
					right->data.emplace_back(r_obj);
					render_script.emplace_hint(
						Utilities::Container::get_last_element_iterator(render_script),
						obj.time, RenderScript{ .r_obj = r_obj, .is_kat = obj.is_kat,
							.left_index = left_index,
							.right_index = right->data.size() - 1 });
				}
				else
				{
					const auto right_index = right->data.empty() ? 0 : right->data.size() - 1;
					left->data.emplace_back(r_obj);
					render_script.emplace_hint(
						Utilities::Container::get_last_element_iterator(render_script),
						obj.time, RenderScript{ .r_obj = r_obj, .is_kat = obj.is_kat,
							.left_index = left->data.size() - 1,
							.right_index = right_index });
				}
			}
		}
	}
}
// D3ST1N4T10N U4KN0WN...