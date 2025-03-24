#include "game/metadata.h" // Header
#include "utilities.h"
#include "rule/file_format.h"
#include "rule/difficulty.h"

//! Metadata::General
void Metadata::General::write(std::ofstream& writer) const
{
	using namespace tfir::General;

	writer << HEADER << '\n';
	writer << AUDIO_FILE << tfir::SEPARATOR << audio_file << '\n';
	writer << MUSIC_DELAY << tfir::SEPARATOR << start_music_delay << '\n';
	writer << PREVIEW_TIMESTAMP << tfir::SEPARATOR << preview_timestamp << '\n';
	writer << EPILEPSY_WARNING << tfir::SEPARATOR << epilepsy_warning << '\n';
	writer << '\n';
}

//! Metadata::Metadata
void Metadata::Metadata::write(std::ofstream& writer) const
{
	using namespace tfir::Metadata;

	// Chuyển unordered_set thành string
	std::string str_tags;
	for (auto& tag : tags)
	{
		if (!str_tags.empty()) str_tags.push_back(' ');
		str_tags.append(tag);
	}

	writer << HEADER << '\n';
	writer << TITLE << tfir::SEPARATOR << title << '\n';
	writer << ARTIST << tfir::SEPARATOR << artist << '\n';
	writer << CREATOR << tfir::SEPARATOR << creator << '\n';
	writer << DIFF_NAME << tfir::SEPARATOR << difficulty_name << '\n';
	writer << SOURCE << tfir::SEPARATOR << source << '\n';
	writer << TAGS << tfir::SEPARATOR << str_tags << '\n';
	writer << '\n';
}

//! Metadata::Difficulty
void Metadata::Difficulty::write(std::ofstream& writer) const
{
	using namespace tfir::Difficulty;

	writer << HEADER << '\n';
	writer << tfir::Difficulty::AR << tfir::SEPARATOR << AR << '\n';
	writer << tfir::Difficulty::HP << tfir::SEPARATOR << HP << '\n';
	writer << tfir::Difficulty::OD << tfir::SEPARATOR << OD << '\n';
	writer << '\n';
}

//! Metadata::CalculatedDifficulty::Approach_Rate
void Metadata::CalculatedDifficulty::Approach_Rate::apply(const float v)
{
	using namespace ::Difficulty::AR;

	value = v;
	if (Utilities::Math::is_equal_float(v, 5))
	{
		preempt_time = PREEMPT_AR5;
		fade_in_time = FADE_IN_AR5;
	}
	else if (v < 5)
	{
		preempt_time = PREEMPT_AR5 + 600 * (5 - v) / 5;
		fade_in_time = FADE_IN_AR5 + 400 * (5 - v) / 5;
	}
	else // v > 5
	{
		preempt_time = PREEMPT_AR5 - 750 * (v - 5) / 5;
		fade_in_time = FADE_IN_AR5 - 500 * (v - 5) / 5;
	}
}
void Metadata::CalculatedDifficulty::Approach_Rate::apply() { apply(value); }

//! Metadata::CalculatedDifficulty::Overall_Difficulty
void Metadata::CalculatedDifficulty::Overall_Difficulty::apply(const float v)
{
	using namespace ::Difficulty::OD;

	value = v;
	perfect = Base::PERFECT - v * Multiply::PERFECT;
	good = Base::GOOD - v * Multiply::GOOD;
	bad = Base::BAD - v * Multiply::BAD;
}
void Metadata::CalculatedDifficulty::Overall_Difficulty::apply() { apply(value); }

//! Metadata::CalculatedDifficulty::HP_Drain_Rate
void Metadata::CalculatedDifficulty::HP_Drain_Rate::apply(const float v)
{
	// Follow Rules: https://osu.ppy.sh/wiki/en/Gameplay/Health#osu!taiko
	// TODO: later, i have no idea with math
	value = v;
}
void Metadata::CalculatedDifficulty::HP_Drain_Rate::apply() { apply(value); }

//! Metadata::CalculatedDifficulty
void Metadata::CalculatedDifficulty::print(std::ofstream& writer) const
{
	using namespace  tfir::Difficulty;

	writer << HEADER << '\n';
	writer << tfir::Difficulty::AR << tfir::SEPARATOR << AR.value << '\n';
	writer << tfir::Difficulty::HP << tfir::SEPARATOR << HP.value << '\n';
	writer << tfir::Difficulty::OD << tfir::SEPARATOR << OD.value << '\n';
	writer << '\n';
}
Metadata::CalculatedDifficulty::CalculatedDifficulty(const Difficulty basic)
{
	AR.apply(basic.AR);
	OD.apply(basic.OD);
	HP.apply(basic.HP);
}