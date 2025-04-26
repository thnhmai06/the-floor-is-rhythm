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

	struct Mapset
	{
		Metadata::General general;
		Metadata::Metadata metadata;
		std::unique_ptr<Metadata::CalculatedDifficulty> calculated_difficulty;
		HitObjects::HitObjects hit_objects;
		TimingPoints::TimingPoints timing_points;
		OsuParser::Beatmap::Objects::Event::Events events;
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
			void calculate(const Mapset& beatmap);

			Stats() = default;
			explicit Stats(const Mapset& completed_beatmap) { calculate(completed_beatmap); }
		} stats;
	private:
		void parse(std::unordered_map<std::string, std::vector<std::string>>& sections);

	public:
		explicit Mapset(const std::filesystem::path& path);
	};
}