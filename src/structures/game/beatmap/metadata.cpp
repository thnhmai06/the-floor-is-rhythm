#include "structures/game/beatmap/metadata.h" // Header
#include <fstream>
#include "utilities.h"
#include "format/file.h"

namespace Structures::Game::Beatmap::Metadata
{
	static constexpr int32_t MINIMUM_LINE_CHARACTERS = 3;

	//! Metadata::General
	void General::read(const std::vector<std::string>& contents)
	{
		for (const auto& line : contents)
		{
			if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
			if (line.find(Format::FileFormat::Beatmap::SEPARATOR) == std::string::npos) continue;
			const auto content = Utilities::String::split(line, Format::FileFormat::Beatmap::SEPARATOR, true);
			if (content.size() <= 1) continue;

			// [0] là key, [1] là value
			if (content[0] == Format::FileFormat::Beatmap::General::AUDIO_FILE)
				audio_file = content[1];
			else if (content[0] == Format::FileFormat::Beatmap::General::MUSIC_DELAY)
				start_music_delay = std::stoi(content[1]);
			else if (content[0] == Format::FileFormat::Beatmap::General::PREVIEW_TIMESTAMP)
				preview_timestamp = std::stoi(content[1]);
			else if (content[0] == Format::FileFormat::Beatmap::General::EPILEPSY_WARNING)
				epilepsy_warning = std::stoi(content[1]);
		}
	}
	void General::write(std::ofstream& writer) const
	{
		using namespace Format::FileFormat::Beatmap::General;

		writer << HEADER << '\n';
		writer << AUDIO_FILE << Format::FileFormat::Beatmap::SEPARATOR << audio_file << '\n';
		writer << MUSIC_DELAY << Format::FileFormat::Beatmap::SEPARATOR << start_music_delay << '\n';
		writer << PREVIEW_TIMESTAMP << Format::FileFormat::Beatmap::SEPARATOR << preview_timestamp << '\n';
		writer << EPILEPSY_WARNING << Format::FileFormat::Beatmap::SEPARATOR << epilepsy_warning << '\n';
		writer << '\n';
	}

	//! Metadata::Metadata
	void Metadata::read(const std::vector<std::string>& contents)
	{
		for (const auto& line : contents)
		{
			if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
			if (line.find(Format::FileFormat::Beatmap::SEPARATOR) == std::string::npos) continue;
			const auto content = Utilities::String::split(line, Format::FileFormat::Beatmap::SEPARATOR, true);
			if (content.size() <= 1) continue;
			// [0] là key, [1] là value
			if (content[0] == Format::FileFormat::Beatmap::Metadata::TITLE)
				title = content[1];
			else if (content[0] == Format::FileFormat::Beatmap::Metadata::ARTIST)
				artist = content[1];
			else if (content[0] == Format::FileFormat::Beatmap::Metadata::CREATOR)
				creator = content[1];
			else if (content[0] == Format::FileFormat::Beatmap::Metadata::DIFF_NAME)
				difficulty_name = content[1];
			else if (content[0] == Format::FileFormat::Beatmap::Metadata::SOURCE)
				source = content[1];
			else if (content[0] == Format::FileFormat::Beatmap::Metadata::TAGS)
				tags = Utilities::String::split(content[1], ' ');
		}
	}
	void Metadata::write(std::ofstream& writer) const
	{
		using namespace Format::FileFormat::Beatmap::Metadata;

		// Chuyển unordered_set thành string
		std::string str_tags;
		for (auto& tag : tags)
		{
			if (!str_tags.empty()) str_tags.push_back(' ');
			str_tags.append(tag);
		}

		writer << HEADER << '\n';
		writer << TITLE << Format::FileFormat::Beatmap::SEPARATOR << title << '\n';
		writer << ARTIST << Format::FileFormat::Beatmap::SEPARATOR << artist << '\n';
		writer << CREATOR << Format::FileFormat::Beatmap::SEPARATOR << creator << '\n';
		writer << DIFF_NAME << Format::FileFormat::Beatmap::SEPARATOR << difficulty_name << '\n';
		writer << SOURCE << Format::FileFormat::Beatmap::SEPARATOR << source << '\n';
		writer << TAGS << Format::FileFormat::Beatmap::SEPARATOR << str_tags << '\n';
		writer << '\n';
	}

	//! Metadata::Difficulty
	void Difficulty::read(const std::vector<std::string>& contents)
	{
		for (const auto& line : contents)
		{
			if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
			if (line.find(Format::FileFormat::Beatmap::SEPARATOR) == std::string::npos) continue;
			const auto content = Utilities::String::split(line, Format::FileFormat::Beatmap::SEPARATOR, true);
			if (content.size() <= 1) continue;
			// [0] là key, [1] là value
			if (content[0] == Format::FileFormat::Beatmap::Difficulty::HP)
				hp = std::stof(content[1]);
			else if (content[0] == Format::FileFormat::Beatmap::Difficulty::OD)
				od = std::stof(content[1]);
			else if (content[0] == Format::FileFormat::Beatmap::Difficulty::VELOCITY)
				velocity = std::stof(content[1]);
		}
	}
	void Difficulty::write(std::ofstream& writer) const
	{
		using namespace Format::FileFormat::Beatmap::Difficulty;

		writer << HEADER << '\n';
		writer << HP << Format::FileFormat::Beatmap::SEPARATOR << hp << '\n';
		writer << OD << Format::FileFormat::Beatmap::SEPARATOR << od << '\n';
		writer << VELOCITY << Format::FileFormat::Beatmap::SEPARATOR << velocity << '\n';
		writer << '\n';
	}

	//! Metadata::CalculatedDifficulty
	// ::OverallDifficulty
	void CalculatedDifficulty::OverallDifficulty::apply(const float& v)
	{
		value = v;
		perfect = Config::GameConfig::Difficulty::OD::Base::PERFECT - v * Config::GameConfig::Difficulty::OD::Multiply::PERFECT;
		great = Config::GameConfig::Difficulty::OD::Base::GOOD - v * Config::GameConfig::Difficulty::OD::Multiply::GOOD;
		bad = Config::GameConfig::Difficulty::OD::Base::BAD - v * Config::GameConfig::Difficulty::OD::Multiply::BAD;
	}
	int16_t CalculatedDifficulty::OverallDifficulty::get_score(const int64_t& click_moment, const int64_t& hit_object_time) const
	{
		if (Utilities::Math::in_range(static_cast<float>(hit_object_time), perfect, static_cast<float>(click_moment)))
			return 300;
		if (Utilities::Math::in_range(static_cast<float>(hit_object_time), great, static_cast<float>(click_moment)))
			return 100;
		if (Utilities::Math::in_range(static_cast<float>(hit_object_time), bad, static_cast<float>(click_moment)))
			return 50;
		if (Utilities::Math::in_range(static_cast<float>(hit_object_time), miss, static_cast<float>(click_moment)))
			return 0;
		return -1; // chưa tới tầm với
	}
	// ::HPDrainRate
	void CalculatedDifficulty::HPDrainRate::apply(const float& v)
	{
		// Follow Rules: https://osu.ppy.sh/wiki/en/Gameplay/Health#osu!taiko
		// TODO: later, I have no idea with math
		value = v;
	}
	void CalculatedDifficulty::Velocity::apply(const float& v)
	{
		value = v;
		speed = v * Config::GameConfig::Difficulty::Velocity::BASE_SPEED;
	}
	// ::
	void CalculatedDifficulty::write(std::ofstream& writer) const
	{
		using namespace  Format::FileFormat::Beatmap::Difficulty;

		writer << HEADER << '\n';
		writer << HP << Format::FileFormat::Beatmap::SEPARATOR << hp.value << '\n';
		writer << OD << Format::FileFormat::Beatmap::SEPARATOR << od.value << '\n';
		writer << VELOCITY << Format::FileFormat::Beatmap::SEPARATOR << velocity.value << '\n';
		writer << '\n';
	}
	void CalculatedDifficulty::apply(const Difficulty& basic)
	{
		od.apply(basic.od);
		hp.apply(basic.hp);
		velocity.apply(basic.velocity);
	}
}