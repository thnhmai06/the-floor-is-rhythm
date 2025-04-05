#include "structures/game/metadata.h" // Header
#include <fstream>
#include "config.h"
#include "utilities.h"
#include "format/file.h"

using namespace GameObjects::Metadata;
static constexpr int32_t MINIMUM_LINE_CHARACTERS = 3;

//! Metadata::General
void General::read(const std::vector<std::string>& contents)
{
	for (const auto& line : contents)
	{
		if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
		if (line.find(FileFormat::Beatmap::SEPARATOR) == std::string::npos) continue;
		const auto content = Utilities::String::split(line, FileFormat::Beatmap::SEPARATOR, true);
		if (content.size() <= 1) continue;

		// [0] là key, [1] là value
		if (content[0] == FileFormat::Beatmap::General::AUDIO_FILE)
			audio_file = content[1];
		else if (content[0] == FileFormat::Beatmap::General::MUSIC_DELAY)
			start_music_delay = std::stoi(content[1]);
		else if (content[0] == FileFormat::Beatmap::General::PREVIEW_TIMESTAMP)
			preview_timestamp = std::stoi(content[1]);
		else if (content[0] == FileFormat::Beatmap::General::EPILEPSY_WARNING)
			epilepsy_warning = std::stoi(content[1]);
	}
}
void General::write(std::ofstream& writer) const
{
	using namespace FileFormat::Beatmap::General;

	writer << HEADER << '\n';
	writer << AUDIO_FILE << FileFormat::Beatmap::SEPARATOR << audio_file << '\n';
	writer << MUSIC_DELAY << FileFormat::Beatmap::SEPARATOR << start_music_delay << '\n';
	writer << PREVIEW_TIMESTAMP << FileFormat::Beatmap::SEPARATOR << preview_timestamp << '\n';
	writer << EPILEPSY_WARNING << FileFormat::Beatmap::SEPARATOR << epilepsy_warning << '\n';
	writer << '\n';
}

//! Metadata::Metadata
void Metadata::read(const std::vector<std::string>& contents)
{
	for (const auto& line : contents)
	{
		if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
		if (line.find(FileFormat::Beatmap::SEPARATOR) == std::string::npos) continue;
		const auto content = Utilities::String::split(line, FileFormat::Beatmap::SEPARATOR, true);
		if (content.size() <= 1) continue;
		// [0] là key, [1] là value
		if (content[0] == FileFormat::Beatmap::Metadata::TITLE)
			title = content[1];
		else if (content[0] == FileFormat::Beatmap::Metadata::ARTIST)
			artist = content[1];
		else if (content[0] == FileFormat::Beatmap::Metadata::CREATOR)
			creator = content[1];
		else if (content[0] == FileFormat::Beatmap::Metadata::DIFF_NAME)
			difficulty_name = content[1];
		else if (content[0] == FileFormat::Beatmap::Metadata::SOURCE)
			source = content[1];
		else if (content[0] == FileFormat::Beatmap::Metadata::TAGS)
			tags = Utilities::String::split(content[1], ' ');
	}
}
void Metadata::write(std::ofstream& writer) const
{
	using namespace FileFormat::Beatmap::Metadata;

	// Chuyển unordered_set thành string
	std::string str_tags;
	for (auto& tag : tags)
	{
		if (!str_tags.empty()) str_tags.push_back(' ');
		str_tags.append(tag);
	}

	writer << HEADER << '\n';
	writer << TITLE << FileFormat::Beatmap::SEPARATOR << title << '\n';
	writer << ARTIST << FileFormat::Beatmap::SEPARATOR << artist << '\n';
	writer << CREATOR << FileFormat::Beatmap::SEPARATOR << creator << '\n';
	writer << DIFF_NAME << FileFormat::Beatmap::SEPARATOR << difficulty_name << '\n';
	writer << SOURCE << FileFormat::Beatmap::SEPARATOR << source << '\n';
	writer << TAGS << FileFormat::Beatmap::SEPARATOR << str_tags << '\n';
	writer << '\n';
}

//! Metadata::Difficulty
void Difficulty::read(const std::vector<std::string>& contents)
{
	for (const auto& line : contents)
	{
		if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
		if (line.find(FileFormat::Beatmap::SEPARATOR) == std::string::npos) continue;
		const auto content = Utilities::String::split(line, FileFormat::Beatmap::SEPARATOR, true);
		if (content.size() <= 1) continue;
		// [0] là key, [1] là value
		if (content[0] == FileFormat::Beatmap::Difficulty::HP)
			hp = std::stof(content[1]);
		else if (content[0] == FileFormat::Beatmap::Difficulty::OD)
			od = std::stof(content[1]);
		else if (content[0] == FileFormat::Beatmap::Difficulty::VELOCITY)
			velocity = std::stof(content[1]);
	}
}
void Difficulty::write(std::ofstream& writer) const
{
	using namespace FileFormat::Beatmap::Difficulty;

	writer << HEADER << '\n';
	writer << HP << FileFormat::Beatmap::SEPARATOR << hp << '\n';
	writer << OD << FileFormat::Beatmap::SEPARATOR << od << '\n';
	writer << VELOCITY << FileFormat::Beatmap::SEPARATOR << velocity << '\n';
	writer << '\n';
}

//! Metadata::CalculatedDifficulty
// ::OverallDifficulty
void CalculatedDifficulty::OverallDifficulty::apply(const float& v)
{
	value = v;
	perfect = GameConfig::Difficulty::OD::Base::PERFECT - v * GameConfig::Difficulty::OD::Multiply::PERFECT;
	good = GameConfig::Difficulty::OD::Base::GOOD - v * GameConfig::Difficulty::OD::Multiply::GOOD;
	bad = GameConfig::Difficulty::OD::Base::BAD - v * GameConfig::Difficulty::OD::Multiply::BAD;
}
// ::HPDrainRate
void CalculatedDifficulty::HPDrainRate::apply(const float& v)
{
	// Follow Rules: https://osu.ppy.sh/wiki/en/Gameplay/Health#osu!taiko
	// TODO: later, i have no idea with math
	value = v;
}
void CalculatedDifficulty::Velocity::apply(const float& v)
{
	value = v;
	speed = v * GameConfig::Difficulty::Velocity::BASE_SPEED;
}
// ::
void CalculatedDifficulty::write(std::ofstream& writer) const
{
	using namespace  FileFormat::Beatmap::Difficulty;

	writer << HEADER << '\n';
	writer << HP << FileFormat::Beatmap::SEPARATOR << hp.value << '\n';
	writer << OD << FileFormat::Beatmap::SEPARATOR << od.value << '\n';
	writer << VELOCITY << FileFormat::Beatmap::SEPARATOR << velocity.value << '\n';
	writer << '\n';
}
void CalculatedDifficulty::apply(const Difficulty& basic)
{
	od.apply(basic.od);
	hp.apply(basic.hp);
	velocity.apply(basic.velocity);
}