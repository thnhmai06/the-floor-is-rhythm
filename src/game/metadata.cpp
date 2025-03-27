#include "game/metadata.h" // Header
#include "utilities.h"
#include "rule/file_format.h"
#include "rule/difficulty.h"

static constexpr int32_t MINIMUM_LINE_CHARACTERS = 3;

//! Metadata::General
void Metadata::General::read(const std::vector<std::string>& contents)
{
	for (const auto& line : contents)
	{
		if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
		if (line.find(tfir::SEPARATOR) == std::string::npos) continue;
		const auto content = Utilities::String::split(line, tfir::SEPARATOR, true);
		if (content.size() <= 1) continue;

		// [0] là key, [1] là value
		if (content[0] == tfir::General::AUDIO_FILE)
			audio_file = content[1];
		else if (content[0] == tfir::General::MUSIC_DELAY)
			start_music_delay = std::stoi(content[1]);
		else if (content[0] == tfir::General::PREVIEW_TIMESTAMP)
			preview_timestamp = std::stoi(content[1]);
		else if (content[0] == tfir::General::EPILEPSY_WARNING)
			epilepsy_warning = std::stoi(content[1]);
	}
}
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
void Metadata::Metadata::read(const std::vector<std::string>& contents)
{
	for (const auto& line : contents)
	{
		if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
		if (line.find(tfir::SEPARATOR) == std::string::npos) continue;
		const auto content = Utilities::String::split(line, tfir::SEPARATOR, true);
		if (content.size() <= 1) continue;
		// [0] là key, [1] là value
		if (content[0] == tfir::Metadata::TITLE)
			title = content[1];
		else if (content[0] == tfir::Metadata::ARTIST)
			artist = content[1];
		else if (content[0] == tfir::Metadata::CREATOR)
			creator = content[1];
		else if (content[0] == tfir::Metadata::DIFF_NAME)
			difficulty_name = content[1];
		else if (content[0] == tfir::Metadata::SOURCE)
			source = content[1];
		else if (content[0] == tfir::Metadata::TAGS)
			tags = Utilities::String::split(content[1], ' ');
	}
}
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
void Metadata::Difficulty::read(const std::vector<std::string>& contents)
{
	for (const auto& line : contents)
	{
		if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
		if (line.find(tfir::SEPARATOR) == std::string::npos) continue;
		const auto content = Utilities::String::split(line, tfir::SEPARATOR, true);
		if (content.size() <= 1) continue;
		// [0] là key, [1] là value
		if (content[0] == tfir::Difficulty::HP)
			HP = std::stof(content[1]);
		else if (content[0] == tfir::Difficulty::OD)
			OD = std::stof(content[1]);
	}
}
void Metadata::Difficulty::write(std::ofstream& writer) const
{
	using namespace tfir::Difficulty;

	writer << HEADER << '\n';
	writer << tfir::Difficulty::AR << tfir::SEPARATOR << AR << '\n';
	writer << tfir::Difficulty::HP << tfir::SEPARATOR << HP << '\n';
	writer << tfir::Difficulty::OD << tfir::SEPARATOR << OD << '\n';
	writer << '\n';
}
//! Metadata::CalculatedDifficulty::Overall_Difficulty
void Metadata::CalculatedDifficulty::Overall_Difficulty::apply(const float v)
{
	using namespace ::Difficulty::OD;

	value = v;
	perfect = Base::PERFECT - v * Multiply::PERFECT;
	good = Base::GOOD - v * Multiply::GOOD;
	bad = Base::BAD - v * Multiply::BAD;
}
//! Metadata::CalculatedDifficulty::HP_Drain_Rate
void Metadata::CalculatedDifficulty::HP_Drain_Rate::apply(const float v)
{
	// Follow Rules: https://osu.ppy.sh/wiki/en/Gameplay/Health#osu!taiko
	// TODO: later, i have no idea with math
	value = v;
}
//! Metadata::CalculatedDifficulty
void Metadata::CalculatedDifficulty::write(std::ofstream& writer) const
{
	using namespace  tfir::Difficulty;

	writer << HEADER << '\n';
	writer << tfir::Difficulty::HP << tfir::SEPARATOR << HP.value << '\n';
	writer << tfir::Difficulty::OD << tfir::SEPARATOR << OD.value << '\n';
	writer << '\n';
}
void Metadata::CalculatedDifficulty::apply(const Difficulty& basic)
{
	OD.apply(basic.OD);
	HP.apply(basic.HP);
}