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

constexpr std::string SPACING = "=";
constexpr std::string ENDED = ""; // TODO: Nghĩ ra một cái End mỗi category thật ngầu lòi :v

/**
  *
  * @brief Kiểm tra biến có bằng giá trị không? (float)
  * @param var Biến cần so sánh
  * @param value Số cần đối chiếu
  * @return bool: var == value với sai số tối thiểu EPSILON cho phép
  */
static bool is_equal(const float& var, const float value)
{
	constexpr float EPSILON = 0.01;  // NOLINT(clang-diagnostic-implicit-float-conversion)
	return abs(var - value) <= EPSILON;
}

struct General
{
private:
	constexpr std::string GENERAL = "[General]";
	constexpr std::string AUDIO_FILE = "AudioFilename";
	constexpr std::string MUSIC_DELAY = "AudioLeadIn";
	constexpr std::string PREVIEW_TIMESTAMP = "PreviewTime";
	constexpr std::string EPILEPSY_WARNING = "EpilepsyWarning";

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
	std::string title, artist, creator, version, source, convert_from;
	//std::vector<std::string> tags; TODO: Query tag DS
};

/**
 * @class Difficulty
 * @ingroup beatmap
 * @brief Biểu diễn độ khó của beatmap
 */
struct Difficulty
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
	private:
		constexpr float PREEMPT_AR5 = 1200;
		constexpr float FADE_IN_AR5 = 800;

	public:
		float value = -1, preempt_time = -1, fade_in_time = -1;

		void apply(const float value)
		{
			this->value = value;

			if (is_equal(value, 5))
			{
				preempt_time = PREEMPT_AR5;
				fade_in_time = FADE_IN_AR5;
			}
			else if (value < 5)
			{
				preempt_time = PREEMPT_AR5 + 600 * float(5 - value) / 5;
				fade_in_time = FADE_IN_AR5 + 400 * float(5 - value) / 5;
			}
			else // value > 5
			{
				preempt_time = PREEMPT_AR5 - 750 * float(value - 5) / 5;
				fade_in_time = FADE_IN_AR5 - 500 * float(value - 5) / 5;
			}
		}

		Approach_Rate() = default;
		Approach_Rate(const float value) { apply(value); }
	} ar;

	struct Health
	{
		// TODO: here
	};

	/**
	 * @class Overall_Difficulty
	 * @ingroup beatmap difficulty
	 * @brief Biểu diễn OD của map (Khoảng thời gian chấp nhận là chính xác hơn).
	 */
	struct Overall_Difficulty
	{
		// Follow: https://osu.ppy.sh/wiki/en/Beatmap/Overall_difficulty
	private:
		constexpr int BASE_PERFECT = 80; // 300
		constexpr int BASE_GOOD = 140; // 100
		constexpr int BASE_BAD = 200; // 50
		constexpr int MULTIPLY_PERFECT = 6;
		constexpr int MULTIPLY_GOOD = 8;
		constexpr int MULTIPLY_BAD = 10;
		// other = miss
	public:
		float value = -1, perfect = -1, good = -1, bad = -1;

		void apply(const float value)
		{
			this->value = value;
			perfect = BASE_PERFECT - value * MULTIPLY_PERFECT;
			good = BASE_GOOD - value * MULTIPLY_GOOD;
			bad = BASE_BAD - value * MULTIPLY_BAD;
		}

		Overall_Difficulty() = default;
		Overall_Difficulty(const float value) { apply(value); }
	} od;

	/**
	 * @class HP_Drain_Rate
	 * @ingroup beatmap difficulty
	 * @brief Biểu diễn HP của map (Độ tụt HP khi bấm không chính xác).
	 */
	struct HP_Drain_Rate
		// Follow Rules: https://osu.ppy.sh/wiki/en/Gameplay/Health#osu!taiko
	{
		// TODO: later, i have no idea with math
	};
};

#endif
