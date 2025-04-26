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
		std::string to_string() const;

		General() = default;
		explicit General(const std::vector<std::string>& contents) { read(contents); }
		friend std::ostream& operator<<(std::ostream& os, const General& general);
	};
	struct Metadata
	{
		std::string title, artist, creator, difficulty_name, source;
		std::vector<std::string> tags;
		void read(const std::vector<std::string>& contents);
		std::string to_string() const;

		Metadata() = default;
		explicit Metadata(const std::vector<std::string>& contents) { read(contents); }
		friend std::ostream& operator<<(std::ostream& os, const Metadata& metadata);
	};

	struct CalculatedDifficulty;
	struct Difficulty
	{
		float od = -1, hp = -1, vl = 1.4f;
		void read(const std::vector<std::string>& contents);
		[[nodiscard]] std::string to_string() const;
		CalculatedDifficulty calculate() const;

		Difficulty() = default;
		explicit Difficulty(const std::vector<std::string>& contents) { read(contents); }
		friend std::ostream& operator<<(std::ostream& os, const Difficulty& difficulty);
	};

	struct CalculatedDifficulty : Difficulty
	{
		struct OverallDifficulty
		{
		protected:
			const float* value = nullptr;
			static void link(const float* od);

			friend CalculatedDifficulty;
		public:
			float get_perfect() const;
			float get_good() const;
			float get_bad() const;
			static float get_miss();
			[[nodiscard]] int16_t get_score(bool is_clicked, const float& click_moment, const int64_t& hit_object_time) const;

			explicit OverallDifficulty(const float* value);
		} overall_difficulty;
		struct HPDrainRate
		{
		protected:
			const float* value = nullptr;

			static void link(const float* hp);
			friend CalculatedDifficulty;
		public:
			float get_drain_rate() const;
			[[nodiscard]] float get_gained_health(const int16_t& note_score, const unsigned long& current_combo) const;

			explicit HPDrainRate(const float* value) { link(value); }
		} hp_drain_rate;
		struct Velocity
		{
		protected:
			const float* value = nullptr;

			static void link(const float* vl);
			friend CalculatedDifficulty;
		public:
			float get_pixel_speed() const;
			explicit Velocity(const float* value) { link(value); }
		} velocity;

		explicit CalculatedDifficulty(const Difficulty& basic)
			: Difficulty(basic), overall_difficulty(&od), hp_drain_rate(&hp), velocity(&vl)
		{
		}
	};
}