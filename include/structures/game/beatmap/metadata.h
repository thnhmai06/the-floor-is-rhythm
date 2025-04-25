#pragma once
#include <string>
#include <unordered_set>
#include "config.h"
#include "structures/types.h"

namespace Structures::Game::Beatmap::Metadata
{
	struct General
	{
		std::string audio_file;
		int32_t start_music_delay = 0;
		int32_t preview_timestamp = 0;
		bool epilepsy_warning = false;
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		General() = default;
		explicit General(const std::vector<std::string>& contents) { read(contents); }
	};
	struct Metadata
	{
		std::string title, artist, creator, difficulty_name, source;
		std::vector<std::string> tags;
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		Metadata() = default;
		explicit Metadata(const std::vector<std::string>& contents) { read(contents); }

	};
	struct Difficulty
	{
		float od = -1, hp = -1, velocity = 1.4f;
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		Difficulty() = default;
		explicit Difficulty(const std::vector<std::string>& contents) { read(contents); }
	};

	struct CalculatedDifficulty
	{
		struct OverallDifficulty
		{
		private:
			float value = 5;

			friend CalculatedDifficulty;
		public:
			float perfect = -1, great = -1, bad = -1, miss = Config::GameConfig::Difficulty::OD::Base::MISS;

			void apply(const float& v);
			void apply() { apply(value); }
			[[nodiscard]] int16_t get_score(bool is_clicked,
				const float& click_moment, const int64_t& hit_object_time,
				const Types::Game::Direction::Direction& required_direction,
				const Types::Game::Direction::Direction& current_direction) const;

			OverallDifficulty() = default;
			explicit OverallDifficulty(const float value) { apply(value); }
		} od;
		struct HealthPoint
		{
		protected:
			float drain;

		private:
			float value = 5;

			friend CalculatedDifficulty;

		public:
			void apply(const float& v);
			void apply() { apply(value); }
			[[nodiscard]] float get_note_hp(const int16_t& note_score, const unsigned long& current_combo) const;

			HealthPoint() = default;
			explicit HealthPoint(const float value) { apply(value); }
		} hp;
		struct Velocity
		{
		private:
			float value = 1.4f;

			friend CalculatedDifficulty;
		public:
			float speed;
			void apply(const float& v);
			void apply() { apply(value); }

			Velocity() { apply(value); }
			explicit Velocity(const float value) { apply(value); }
		} velocity;

		void apply(const Difficulty& basic);
		void write(std::ofstream& writer) const;

		CalculatedDifficulty() = default;
		explicit CalculatedDifficulty(const Difficulty& basic) { apply(basic); }
	};
}