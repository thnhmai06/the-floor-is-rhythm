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
		float od = -1, hp = -1, velocity = 1.4f;
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
		 * @class OverallDifficulty
		 * @ingroup beatmap difficulty
		 * @brief Biểu diễn OD của map (Khoảng thời gian chấp nhận là chính xác hơn).
		 */
		struct OverallDifficulty
		{
			// Follow: https://osu.ppy.sh/wiki/en/Beatmap/Overall_difficulty
			float value = -1, perfect = -1, good = -1, bad = -1;

			void apply(const float& v);
			void apply() { apply(value); }

			OverallDifficulty() = default;
			OverallDifficulty(const float value) { apply(value); }
		} od;
		/**
		 * @class HPDrainRate
		 * @ingroup beatmap difficulty
		 * @brief Biểu diễn HP của map (Độ tụt HP khi bấm không chính xác).
		 */
		struct HPDrainRate
		{
			float value = -1;

			void apply(const float& v);
			void apply() { apply(value); }

			HPDrainRate() = default;
			HPDrainRate(const float value) { apply(value); }
		} hp;
		struct Velocity
		{
			float value = 1.4f, speed;
			void apply(const float& v);
			void apply() { apply(value); }

			Velocity() = default;
			Velocity(const float value) { apply(value); }
		} velocity;

		void apply(const Difficulty& basic);
		void write(std::ofstream& writer) const;

		CalculatedDifficulty() = default;
		CalculatedDifficulty(const Difficulty& basic) { apply(basic); }
	};

}