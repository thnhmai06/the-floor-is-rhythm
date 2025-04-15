#pragma once
#include <functional>
#include "structures/game/beatmap/metadata.h"
#include "structures/game/beatmap/hitobject.h"
#include "structures/game/beatmap/timing.h"

namespace Structures::Game::Beatmap
{
	struct Beatmap
	{
		Metadata::General general;
		Metadata::Metadata metadata;
		Metadata::CalculatedDifficulty calculated_difficulty;
		HitObjects::HitObjects hit_objects;
		TimingPoints::TimingPoints timing_points;

		struct Stats
		{
			unsigned long total_floor = 0;
			unsigned long total_slider = 0;
			unsigned long total_combo = 0;

			[[nodiscard]] unsigned long get_total_objects_num() const;
			void calculate(const Beatmap& beatmap);

			Stats() = default;
			explicit Stats(const Beatmap& completed_beatmap) { calculate(completed_beatmap); }
		} stats;

		[[nodiscard]] std::pair<std::queue<const HitObjects::Floor::Action>, std::queue<const HitObjects::Slider::Action>> make_action_queue() const;

		using HitObjectStepFunction = std::function<void()>;
		void for_all_hit_objects(const HitObjectStepFunction& if_floor, const HitObjectStepFunction& if_slider,
			const HitObjects::HitObject*& current_hit_object_ptr) const;
		void for_all_hit_objects(const HitObjectStepFunction& if_floor, const HitObjectStepFunction& if_slider,
			const HitObjects::HitObject*& current_hit_object_ptr,
			float& current_timing_point_velocity, float& current_beat_length) const;
		explicit Beatmap(const char* file_path);
	};
}