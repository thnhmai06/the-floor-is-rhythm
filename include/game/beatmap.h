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
#include <format.h>
#include <unordered_set>
using tfir::SPACING, tfir::AND, tfir::ENDED;

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
	std::string audio_file;
	int start_music_delay = 0;
	int preview_timestamp = 0;
	bool epilepsy_warning = 0;

	void print(std::ofstream& writer) const
	{
		using namespace tfir::General;

		writer << HEADER << '\n';
		writer << AUDIO_FILE << SPACING << audio_file << '\n';
		writer << MUSIC_DELAY << SPACING << start_music_delay << '\n';
		writer << PREVIEW_TIMESTAMP << SPACING << preview_timestamp << '\n';
		writer << EPILEPSY_WARNING << SPACING << epilepsy_warning << '\n';
		writer << ENDED << "\n\n";
	}
};

struct Metadata
{
	std::string title, artist, creator, difficulty_name, source;
	//std::unordered_set<std::string> tags; TODO: Tìm CTDL Phù hợp cho truy vấn tìm kiếm beatmap
	std::vector<std::string> tags;

	void print(std::ofstream& writter) const
	{
		using namespace tfir::Metadata;

		// Chuyển unordered_set thành string
		std::string str_tags;
		for (auto &tag: tags)
		{
			if (!str_tags.empty()) str_tags.push_back(' ');
			str_tags.append(tag);
		}

		writter << HEADER << '\n';
		writter << TITLE << SPACING << title << '\n';
		writter << ARTIST << SPACING << artist << '\n';
		writter << CREATOR << SPACING << creator << '\n';
		writter << DIFF_NAME << SPACING << difficulty_name << '\n';
		writter << SOURCE << SPACING << source << '\n';
		writter << TAGS << SPACING << str_tags << '\n';
		writter << ENDED << "\n\n";
	}
};

struct BasicDifficulty
{
	float ar = -1, od = -1, hp = -1;

	void print(std::ofstream& writer) const
	{
		using namespace tfir::Difficulty;

		writer << HEADER << '\n';
		writer << AR << SPACING << ar << '\n';
		writer << HP << SPACING << hp << '\n';
		writer << OD << SPACING << od << '\n';
		writer << ENDED << "\n\n";
	}
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

		float value = -1, preempt_time = -1, fade_in_time = -1;

		void apply(const float v)
		{
			using namespace tfir::Difficulty::AR_calculation;

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
		// other = miss
	public:
		float value = -1, perfect = -1, good = -1, bad = -1;

		void apply(const float v)
		{
			using namespace tfir::Difficulty::OD_calculation;

			value = v;
			perfect = Base::PERFECT - v * Multiply::PERFECT;
			good = Base::GOOD - v * Multiply::GOOD;
			bad = Base::BAD - v * Multiply::BAD;
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
		using namespace  tfir::Difficulty;

		writer << HEADER << '\n';
		writer << AR << SPACING << ar.value << '\n';
		writer << HP << SPACING << hp.value << '\n';
		writer << OD << SPACING << od.value << '\n';
		writer << ENDED << "\n\n";
	}

	Difficulty() = default;
	Difficulty(const BasicDifficulty basic)
	{
		ar.apply(basic.ar);
		od.apply(basic.od);
		hp.apply(basic.hp);
	}
};

#endif
