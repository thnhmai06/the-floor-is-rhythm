#pragma once
#include <functional>
#include <filesystem>
#include "structures/game/mapset/metadata.h"
#include "structures/game/mapset/hitobject.h"
#include "structures/game/mapset/timing.h"
#include "structures/game/event.h"

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
		std::filesystem::path path;
		Metadata::General general;
		Metadata::Metadata metadata;
		std::unique_ptr<Metadata::CalculatedDifficulty> calculated_difficulty;
		HitObjects::HitObjects hit_objects;
		TimingPoints::TimingPoints timing_points;
		Event::Events events;
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
				unsigned long total_combo = 0;
			} count;

			[[nodiscard]] unsigned long get_total_objects_num() const;
			void calculate(const Mapset& mapset);

			Stats() = default;
			explicit Stats(const Mapset& completed_beatmap) { calculate(completed_beatmap); }
		} stats;
	private:
		void parse(std::unordered_map<std::string, std::vector<std::string>>& sections);
		void load_event_file();

	public:
		explicit Mapset(const std::filesystem::path& path, bool load_event_file = false);
	};
}