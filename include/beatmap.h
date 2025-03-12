/**
 *	@file beatmap.h
 *	@brief Các lớp biểu diễn các thành phần Beatmap
 *	@author Mai Thành (@thnhmai06)
 */
#ifndef BEATMAP_H
#define BEATMAP_H

#include <cmath>
#include <string>

 /**
  *
  * @brief Kiểm tra biến có bằng giá trị không? (float)
  * @param var Biến cần so sánh
  * @param value Số cần đối chiếu
  * @return bool: var == value với sai số tối thiểu EPSILON cho phép
  */
static bool is_equal(const float& var, const float value)
{
	constexpr float EPSILON = 0.01;
	return abs(var - value) <= EPSILON;
}

class General
{
public:
	std::string audio_file;
	int start_music_delay;
	int preview_timestamp;
	bool epilepsy_warning;
};

/**
 * @class Difficulty
 * @ingroup beatmap
 * @brief Biểu diễn độ khó của beatmap
 */
class Difficulty
{
	/**
	 * @class Approach_Rate
	 * @ingroup beatmap difficulty
	 * @brief Biểu diễn AR của map (Thời gian tiếp cận)
	 */
	class Approach_Rate
	{
		// Follow: https://osu.ppy.sh/wiki/en/Beatmap/Approach_rate
		// yeah, this game is "based" on osu! lmfao
	private:
		constexpr float PREEMPT_AR5 = 1200;
		constexpr float FADE_IN_AR5 = 800;

	public:
		float value, preempt_time, fade_in_time;

		explicit Approach_Rate(const float value)
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
	};

	class Health
	{

	};

	/**
	 * @class Overall_Difficulty
	 * @ingroup beatmap difficulty
	 * @brief Biểu diễn OD của map (Khoảng thời gian chấp nhận là chính xác hơn).
	 */
	class Overall_Difficulty
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
		float value, accept_perfect, accept_good, accept_bad;

		explicit Overall_Difficulty(const float value)
		{
			this->value = value;
			accept_perfect = BASE_PERFECT - value * MULTIPLY_PERFECT;
			accept_good = BASE_GOOD - value * MULTIPLY_GOOD;
			accept_bad = BASE_BAD - value * MULTIPLY_BAD;
		}
	};

	/**
	 * @class HP_Drain_Rate
	 * @ingroup beatmap difficulty
	 * @brief Biểu diễn HP của map (Độ tụt HP khi bấm không chính xác).
	 */
	class HP_Drain_Rate
		// Follow Rules: https://osu.ppy.sh/wiki/en/Gameplay/Health#osu!taiko
	{
		// later, i have no idea with math
	};
};

#endif
