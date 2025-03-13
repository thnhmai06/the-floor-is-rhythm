/**
 *	@file beatmap.h
 *	@brief Các lớp biểu diễn các thành phần Beatmap
 *	@author Mai Thành (@thnhmai06)
 */
#ifndef BEATMAP_H
#define BEATMAP_H

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>

constexpr std::string_view SPACING = "=";
constexpr std::string_view ENDED = ";end";
constexpr char AND = ',';

/**
  *
  * @brief Kiểm tra biến có bằng giá trị không? (float)
  * @param var Biến cần so sánh
  * @param value Số cần đối chiếu
  * @return bool: var == value với sai số tối thiểu EPSILON cho phép
  */
static bool is_equal(const float& var, const float value)
{
	const float EPSILON = 0.01;  // NOLINT(clang-diagnostic-implicit-float-conversion)
	return abs(var - value) <= EPSILON;
}

struct General
{
private:
	const std::string_view GENERAL = "[General]";
	const std::string_view AUDIO_FILE = "AudioFilename";
	const std::string_view MUSIC_DELAY = "AudioLeadIn";
	const std::string_view PREVIEW_TIMESTAMP = "PreviewTime";
	const std::string_view EPILEPSY_WARNING = "EpilepsyWarning";

public:
	std::string audio_file;
	int start_music_delay;
	int preview_timestamp;
	bool epilepsy_warning;

	void print(std::ofstream& writer) const
	{
		writer << GENERAL << '\n';
		writer << AUDIO_FILE << SPACING << audio_file << '\n';
		writer << MUSIC_DELAY << SPACING << start_music_delay << '\n';
		writer << PREVIEW_TIMESTAMP << SPACING << preview_timestamp << '\n';
		writer << EPILEPSY_WARNING << SPACING << epilepsy_warning << '\n';
		writer << ENDED << "\n\n";
	}
};

struct Metadata
{
private:
	const std::string_view METADATA = "[Metadata]";
	const std::string_view TITLE = "Title";
	const std::string_view ARTIST = "Artist";
	const std::string_view CREATOR = "Creator";
	const std::string_view DIFF_NAME = "DifficultyName";
	const std::string_view SOURCE = "Source";
	const std::string_view TAGS = "Tags";
public:
	std::string title, artist, creator, difficulty_name, source;
	std::unordered_set<std::string> tags;

	void print(std::ofstream& writter) const
	{
		// Chuyển unordered_set thành string
		std::string str_tags;
		for (auto &tag: tags)
		{
			if (!str_tags.empty()) str_tags.push_back(' ');
			str_tags.append(tag);
		}

		writter << METADATA << '\n';
		writter << TITLE << SPACING << title << '\n';
		writter << ARTIST << SPACING << artist << '\n';
		writter << CREATOR << SPACING << creator << '\n';
		writter << DIFF_NAME << SPACING << difficulty_name << '\n';
		writter << SOURCE << SPACING << source << '\n';
		writter << TAGS << SPACING << str_tags << '\n';
		writter << ENDED << "\n\n";
	}
};

/**
 * @class Difficulty
 * @ingroup beatmap
 * @brief Biểu diễn độ khó của beatmap
 */
struct Difficulty
{
private:
	const std::string_view DIFFICULTY = "[Difficulty]";
	const std::string_view HP = "HPDrainRate";
	const std::string_view OD = "OverallDifficulty";
	const std::string_view AR = "ApproachRate";

public:
	/**
	 * @class Approach_Rate
	 * @ingroup beatmap difficulty
	 * @brief Biểu diễn AR của map (Thời gian tiếp cận)
	 */
	struct Approach_Rate
	{
		// Follow: https://osu.ppy.sh/wiki/en/Beatmap/Approach_rate
		// yeah, this game is "based" on osu! lmfao
	private:
		const float PREEMPT_AR5 = 1200;
		const float FADE_IN_AR5 = 800;

	public:
		float value = -1, preempt_time = -1, fade_in_time = -1;

		void apply(const float v)
		{
			value = v;

			if (is_equal(v, 5))
			{
				preempt_time = PREEMPT_AR5;
				fade_in_time = FADE_IN_AR5;
			}
			else if (v < 5)
			{
				preempt_time = PREEMPT_AR5 + 600 * float(5 - v) / 5;
				fade_in_time = FADE_IN_AR5 + 400 * float(5 - v) / 5;
			}
			else // v > 5
			{
				preempt_time = PREEMPT_AR5 - 750 * float(v - 5) / 5;
				fade_in_time = FADE_IN_AR5 - 500 * float(v - 5) / 5;
			}
		}
		void apply() { apply(value); }

		Approach_Rate() = default;
		Approach_Rate(const float value) { apply(value); }
	} ar;

	/**
	 * @class Overall_Difficulty
	 * @ingroup beatmap difficulty
	 * @brief Biểu diễn OD của map (Khoảng thời gian chấp nhận là chính xác hơn).
	 */
	struct Overall_Difficulty
	{
		// Follow: https://osu.ppy.sh/wiki/en/Beatmap/Overall_difficulty
	private:
		const int BASE_PERFECT = 80; // 300
		const int BASE_GOOD = 140; // 100
		const int BASE_BAD = 200; // 50
		const int MULTIPLY_PERFECT = 6;
		const int MULTIPLY_GOOD = 8;
		const int MULTIPLY_BAD = 10;
		// other = miss
	public:
		float value = -1, perfect = -1, good = -1, bad = -1;

		void apply(const float v)
		{
			value = v;
			perfect = BASE_PERFECT - v * MULTIPLY_PERFECT;
			good = BASE_GOOD - v * MULTIPLY_GOOD;
			bad = BASE_BAD - v * MULTIPLY_BAD;
		}
		void apply() { apply(value); }

		Overall_Difficulty() = default;
		Overall_Difficulty(const float value) { apply(value); }
	} od;

	/**
	 * @class HP_Drain_Rate
	 * @ingroup beatmap difficulty
	 * @brief Biểu diễn HP của map (Độ tụt HP khi bấm không chính xác).
	 */
	struct HP_Drain_Rate
	{
		// Follow Rules: https://osu.ppy.sh/wiki/en/Gameplay/Health#osu!taiko
		// TODO: later, i have no idea with math

		float value = -1;

		void apply(const float v) { value = v; }
		void apply() { apply(value); }

		HP_Drain_Rate() = default;
		HP_Drain_Rate(const float value) { apply(value); }
	} hp;

	void print(std::ofstream& writer) const
	{
		writer << DIFFICULTY << '\n';
		writer << AR << SPACING << ar.value << '\n';
		writer << HP << SPACING << hp.value << '\n';
		writer << OD << SPACING << od.value << '\n';
		writer << ENDED << "\n\n";
	}
};

#endif
