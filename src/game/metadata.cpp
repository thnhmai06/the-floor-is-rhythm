#include "game/metadata.h" // Header
#include "utilities.h"
#include "rule/file_format.h"
#include "rule/difficulty.h"

using tfir::SEPARATOR, tfir::SECTION_END;
using Utilities::Math::is_equal_float;

//! Metadata::General
void Metadata::General::write(std::ofstream& writer) const
{
	using namespace tfir::General;

	writer << HEADER << '\n';
	writer << AUDIO_FILE << SEPARATOR << audio_file << '\n';
	writer << MUSIC_DELAY << SEPARATOR << start_music_delay << '\n';
	writer << PREVIEW_TIMESTAMP << SEPARATOR << preview_timestamp << '\n';
	writer << EPILEPSY_WARNING << SEPARATOR << epilepsy_warning << '\n';
	writer << SECTION_END << "\n\n";
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
	writer << TITLE << SEPARATOR << title << '\n';
	writer << ARTIST << SEPARATOR << artist << '\n';
	writer << CREATOR << SEPARATOR << creator << '\n';
	writer << DIFF_NAME << SEPARATOR << difficulty_name << '\n';
	writer << SOURCE << SEPARATOR << source << '\n';
	writer << TAGS << SEPARATOR << str_tags << '\n';
	writer << SECTION_END << "\n\n";
}

//! Metadata::Difficulty
void Metadata::Difficulty::write(std::ofstream& writer) const
{
	using namespace tfir::Difficulty;

	writer << HEADER << '\n';
	writer << tfir::Difficulty::AR << SEPARATOR << AR << '\n';
	writer << tfir::Difficulty::HP << SEPARATOR << HP << '\n';
	writer << tfir::Difficulty::OD << SEPARATOR << OD << '\n';
	writer << SECTION_END << "\n\n";
}

//! Metadata::CalculatedDifficulty::Approach_Rate
void Metadata::CalculatedDifficulty::Approach_Rate::apply(const float v)
{
	using namespace ::Difficulty::AR;

	value = v;
	if (is_equal_float(v, 5))
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
	writer << tfir::Difficulty::AR << SEPARATOR << AR.value << '\n';
	writer << tfir::Difficulty::HP << SEPARATOR << HP.value << '\n';
	writer << tfir::Difficulty::OD << SEPARATOR << OD.value << '\n';
	writer << SECTION_END << "\n\n";
}
Metadata::CalculatedDifficulty::CalculatedDifficulty(const Difficulty basic)
{
	AR.apply(basic.AR);
	OD.apply(basic.OD);
	HP.apply(basic.HP);
}