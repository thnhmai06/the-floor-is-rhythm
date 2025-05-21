#pragma once
#include <string>
#include "structures/config.h"
#include "core/type.h"

namespace Structures::Game::Beatmap::Metadata
{
	using Core::Type::Game::HitSound::SampleSet;

	struct General : Config::Section
	{
		std::string audio_file;
		int32_t start_music_delay = 0;
		int32_t preview_timestamp = 0;
		bool epilepsy_warning = false;
		bool widescreen_storyboard = false;
		SampleSet sample_set = SampleSet::Normal;

		void read(const std::vector<std::string>& contents) override;
		[[nodiscard]] std::string to_string() const override;

		General() = default;
		explicit General(const std::vector<std::string>& contents) { General::read(contents); }
	};
	struct Metadata : Config::Section
	{
		std::string title, artist, creator, difficulty_name, source;
		std::vector<std::string> tags;
		void read(const std::vector<std::string>& contents) override;
		[[nodiscard]] std::string to_string() const override;

		Metadata() = default;
		explicit Metadata(const std::vector<std::string>& contents);
	};

	struct CalculatedDifficulty;
	struct Difficulty : Config::Section
	{
		float od = -1, hp = -1, vc = 1.4;
		void read(const std::vector<std::string>& contents) override;
		[[nodiscard]] std::string to_string() const override;
		CalculatedDifficulty calculate() const;

		Difficulty() = default;
		explicit Difficulty(const std::vector<std::string>& contents);
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
			[[nodiscard]] Core::Type::Game::Gameplay::NoteScore get_score(bool is_clicked, const int64_t& current_time,
			                                                         const int64_t& hit_object_time) const;

			explicit OverallDifficulty(const float& value);
		} overall_difficulty;
		struct HPDrainRate
		{
		protected:
			const float* value = nullptr;
		public:
			[[nodiscard]] float get_drain_rate() const;
			[[nodiscard]] float get_gained_health(const Core::Type::Game::Gameplay::NoteScore& note_score, const unsigned long& current_combo) const;

			explicit HPDrainRate(const float& value);
		} hp_drain_rate;
		struct Velocity
		{
		protected:
			const float* value = nullptr;

		public:
			[[nodiscard]] double get_real_velocity(const float& timing_velocity) const;
			[[nodiscard]] double get_one_beat_length_pixel(const float& timing_velocity) const;
			[[nodiscard]] double get_one_ms_pixel(const float& timing_velocity, const float& timing_beat_length) const;
			[[nodiscard]] double get_one_pixel_time(const float& timing_velocity, const float& timing_beat_length) const;

			explicit Velocity(const float& value);
		} velocity;

		CalculatedDifficulty(const Difficulty& basic);
		explicit CalculatedDifficulty(const std::vector<std::string>& contents);
	};
}