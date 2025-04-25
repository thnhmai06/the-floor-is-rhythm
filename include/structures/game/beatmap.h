#pragma once
#include <functional>
#include <filesystem>
#include <osu!parser/Parser/Structures/Beatmap/Objects/Event.hpp>
#include "structures/game/beatmap/metadata.h"
#include "structures/game/beatmap/hitobject.h"
#include "structures/game/beatmap/timing.h"

namespace Structures::Game::Beatmap
{
	struct Storyboard
	{
	private:
		void parse(std::unordered_map<std::string, std::vector<std::string>>& sections);

	public:
		OsuParser::Beatmap::Sections::Variable::VariableSection variables;
		OsuParser::Beatmap::Objects::Event::Events events;

		explicit Storyboard(const std::filesystem::path& file);
	};

	struct Beatmap
	{
		Metadata::General general;
		Metadata::Metadata metadata;
		Metadata::CalculatedDifficulty calculated_difficulty;
		HitObjects::HitObjects hit_objects;
		TimingPoints::TimingPoints timing_points;
		OsuParser::Beatmap::Objects::Event::Events events;

	private:
		void parse(std::unordered_map<std::string, std::vector<std::string>>& sections);

	public:
		struct Stats
		{
			struct
			{
				int64_t start_time = 0;
				int64_t start_playing_time = 0;
				int64_t length = 0;
			} time;
			struct
			{
				unsigned long floor = 0;
				unsigned long slider = 0;
				unsigned long total_combo = 0;
			} count;

			[[nodiscard]] unsigned long get_total_objects_num() const;
			void calculate(const Beatmap& beatmap);

			Stats() = default;
			explicit Stats(const Beatmap& completed_beatmap) { calculate(completed_beatmap); }
		} stats;

		using FloorActionQueue = std::queue<std::shared_ptr<const HitObjects::Floor::ActionInfo>>;
		using SliderActionQueue = std::queue<std::shared_ptr<const HitObjects::Slider::ActionInfo>>;
		[[nodiscard]] std::pair<FloorActionQueue, SliderActionQueue> make_action_queue() const;

		using HitObjectStepFunction = std::function<void()>;
		void for_in_hit_objects(const HitObjectStepFunction& if_floor, const HitObjectStepFunction& if_slider,
			const HitObjects::HitObject*& current_hit_object_ptr) const;
		void for_in_hit_objects(const HitObjectStepFunction& if_floor, const HitObjectStepFunction& if_slider,
			const HitObjects::HitObject*& current_hit_object_ptr,
			float& current_timing_point_velocity, float& current_beat_length) const;

		explicit Beatmap(const std::filesystem::path& path);
	};
}