/**
 *	@file metadata.h
 *	@brief Các lớp biểu diễn Metadata của Beatmap
 *	@author Mai Thành (@thnhmai06)
 */
#pragma once
#include <string>
#include <fstream>
#include <unordered_set>

namespace Metadata
{
	//! Raw
	struct General
	{
		std::string audio_file;
		int32_t start_music_delay = 0;
		int32_t preview_timestamp = 0;
		bool epilepsy_warning = 0;
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		General() = default;
		General(const std::vector<std::string>& contents) { read(contents); }
	};
	struct Metadata
	{
		std::string title, artist, creator, difficulty_name, source;
		//std::unordered_set<std::string> tags; TODO: Tìm CTDL Phù hợp cho truy vấn tìm kiếm beatmap
		std::vector<std::string> tags;
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		Metadata() = default;
		Metadata(const std::vector<std::string>& contents) { read(contents); }

	};
	struct Difficulty
	{
		float AR = -1, OD = -1, HP = -1;
		void read(const std::vector<std::string>& contents);
		void write(std::ofstream& writer) const;

		Difficulty() = default;
		Difficulty(const std::vector<std::string>& contents) { read(contents); }
	};

	//! Calculated
	/**
	 * @class CalculatedDifficulty
	 * @ingroup beatmap
	 * @brief Biểu diễn độ khó của beatmap
	 */
	struct CalculatedDifficulty
	{
		/**
		 * @class Approach_Rate
		 * @ingroup beatmap difficulty
		 * @brief Biểu diễn AR của map (Thời gian tiếp cận)
		 */
		struct Approach_Rate
		{
			// Follow: https://osu.ppy.sh/wiki/en/Beatmap/Approach_rate
			// yeah, this game is "based" on osu! lmfao

			float value = -1, preempt_time = -1, fade_in_time = -1;

			void apply(float v);
			void apply() { apply(value); }

			Approach_Rate() = default;
			Approach_Rate(const float value) { apply(value); }
		} AR;

		/**
		 * @class Overall_Difficulty
		 * @ingroup beatmap difficulty
		 * @brief Biểu diễn OD của map (Khoảng thời gian chấp nhận là chính xác hơn).
		 */
		struct Overall_Difficulty
		{
			// Follow: https://osu.ppy.sh/wiki/en/Beatmap/Overall_difficulty
			float value = -1, perfect = -1, good = -1, bad = -1;

			void apply(float v);
			void apply() { apply(value); }

			Overall_Difficulty() = default;
			Overall_Difficulty(const float value) { apply(value); }
		} OD;

		/**
		 * @class HP_Drain_Rate
		 * @ingroup beatmap difficulty
		 * @brief Biểu diễn HP của map (Độ tụt HP khi bấm không chính xác).
		 */
		struct HP_Drain_Rate
		{
			float value = -1;

			void apply(float v);
			void apply() { apply(value); }

			HP_Drain_Rate() = default;
			HP_Drain_Rate(const float value) { apply(value); }
		} HP;

		void apply(const Difficulty& basic);
		void write(std::ofstream& writer) const;

		CalculatedDifficulty() = default;
		CalculatedDifficulty(const Difficulty& basic) { apply(basic); }
	};

}