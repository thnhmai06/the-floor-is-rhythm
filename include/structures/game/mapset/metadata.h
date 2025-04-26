#pragma once
#include <string>
#include "structures/type.hpp"

namespace Structures::Game::Beatmap::Metadata
{
	struct General
	{
		std::string audio_file;
		int32_t start_music_delay = 0;
		int32_t preview_timestamp = 0;
		bool epilepsy_warning = false;
		bool widescreen_storyboard = false;
		Types::Game::HitSound::HitSampleType sample_set = Types::Game::HitSound::HitSampleType::Normal;

		void read(const std::vector<std::string>& contents);
		[[nodiscard]] std::string to_string() const;

		General() = default;
		explicit General(const std::vector<std::string>& contents) { read(contents); }
		friend std::ostream& operator<<(std::ostream& os, const General& general);
	};
	struct Metadata
	{
		std::string title, artist, creator, difficulty_name, source;
		std::vector<std::string> tags;
		void read(const std::vector<std::string>& contents);
		[[nodiscard]] std::string to_string() const;

		Metadata() = default;
		explicit Metadata(const std::vector<std::string>& contents) { read(contents); }
		friend std::ostream& operator<<(std::ostream& os, const Metadata& metadata);
	};

	struct CalculatedDifficulty;
	struct Difficulty
	{
		float od = -1, hp = -1;
		void read(const std::vector<std::string>& contents);
		[[nodiscard]] std::string to_string() const;
		[[nodiscard]] CalculatedDifficulty calculate() const;

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

		public:
			[[nodiscard]] float get_perfect() const;
			[[nodiscard]] float get_good() const;
			[[nodiscard]] float get_bad() const;
			static float get_miss();
			[[nodiscard]] Types::Game::Gameplay::NoteScore get_score(bool is_clicked, const int64_t& current_time,
			                                                         const int64_t& hit_object_time) const;

			explicit OverallDifficulty(const float* value);
		} overall_difficulty;
		struct HPDrainRate
		{
		protected:
			const float* value = nullptr;
		public:
			[[nodiscard]] float get_drain_rate() const;
			[[nodiscard]] float get_gained_health(const Types::Game::Gameplay::NoteScore& note_score, const unsigned long& current_combo) const;

			explicit HPDrainRate(const float* value);
		} hp_drain_rate;

		explicit CalculatedDifficulty(const Difficulty& basic);
	};
}