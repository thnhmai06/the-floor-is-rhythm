#include "structures/game/beatmap/metadata.h" // Header
#include <fstream>
#include "utilities.hpp"
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
			if (line.find(Format::File::Beatmap::SEPARATOR) == std::string::npos) continue;
			const auto content = Utilities::String::split(line, Format::File::Beatmap::SEPARATOR, true);
			if (content.size() <= 1) continue;

			// [0] là key, [1] là value
			if (content[0] == Format::File::Beatmap::General::AUDIO_FILE)
				audio_file = content[1];
			else if (content[0] == Format::File::Beatmap::General::MUSIC_DELAY)
				start_music_delay = std::stoi(content[1]);
			else if (content[0] == Format::File::Beatmap::General::PREVIEW_TIMESTAMP)
				preview_timestamp = std::stoi(content[1]);
			else if (content[0] == Format::File::Beatmap::General::EPILEPSY_WARNING)
				epilepsy_warning = std::stoi(content[1]);
		}
	}
	void General::write(std::ofstream& writer) const
	{
		using namespace Format::File::Beatmap::General;

		writer << HEADER << '\n';
		writer << AUDIO_FILE << Format::File::Beatmap::SEPARATOR << audio_file << '\n';
		writer << MUSIC_DELAY << Format::File::Beatmap::SEPARATOR << start_music_delay << '\n';
		writer << PREVIEW_TIMESTAMP << Format::File::Beatmap::SEPARATOR << preview_timestamp << '\n';
		writer << EPILEPSY_WARNING << Format::File::Beatmap::SEPARATOR << epilepsy_warning << '\n';
		writer << '\n';
	}

	//! Metadata::Metadata
	void Metadata::read(const std::vector<std::string>& contents)
	{
		for (const auto& line : contents)
		{
			if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
			if (line.find(Format::File::Beatmap::SEPARATOR) == std::string::npos) continue;
			const auto content = Utilities::String::split(line, Format::File::Beatmap::SEPARATOR, true);
			if (content.size() <= 1) continue;
			// [0] là key, [1] là value
			if (content[0] == Format::File::Beatmap::Metadata::TITLE)
				title = content[1];
			else if (content[0] == Format::File::Beatmap::Metadata::ARTIST)
				artist = content[1];
			else if (content[0] == Format::File::Beatmap::Metadata::CREATOR)
				creator = content[1];
			else if (content[0] == Format::File::Beatmap::Metadata::DIFF_NAME)
				difficulty_name = content[1];
			else if (content[0] == Format::File::Beatmap::Metadata::SOURCE)
				source = content[1];
			else if (content[0] == Format::File::Beatmap::Metadata::TAGS)
				tags = Utilities::String::split(content[1], ' ');
		}
	}
	void Metadata::write(std::ofstream& writer) const
	{
		using namespace Format::File::Beatmap::Metadata;

		// Chuyển unordered_set thành string
		std::string str_tags;
		for (auto& tag : tags)
		{
			if (!str_tags.empty()) str_tags.push_back(' ');
			str_tags.append(tag);
		}

		writer << HEADER << '\n';
		writer << TITLE << Format::File::Beatmap::SEPARATOR << title << '\n';
		writer << ARTIST << Format::File::Beatmap::SEPARATOR << artist << '\n';
		writer << CREATOR << Format::File::Beatmap::SEPARATOR << creator << '\n';
		writer << DIFF_NAME << Format::File::Beatmap::SEPARATOR << difficulty_name << '\n';
		writer << SOURCE << Format::File::Beatmap::SEPARATOR << source << '\n';
		writer << TAGS << Format::File::Beatmap::SEPARATOR << str_tags << '\n';
		writer << '\n';
	}

	//! Metadata::Difficulty
	void Difficulty::read(const std::vector<std::string>& contents)
	{
		for (const auto& line : contents)
		{
			if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
			if (line.find(Format::File::Beatmap::SEPARATOR) == std::string::npos) continue;
			const auto content = Utilities::String::split(line, Format::File::Beatmap::SEPARATOR, true);
			if (content.size() <= 1) continue;
			// [0] là key, [1] là value
			if (content[0] == Format::File::Beatmap::Difficulty::HP)
				hp = std::stof(content[1]);
			else if (content[0] == Format::File::Beatmap::Difficulty::OD)
				od = std::stof(content[1]);
			else if (content[0] == Format::File::Beatmap::Difficulty::VELOCITY)
				velocity = std::stof(content[1]);
		}
	}
	void Difficulty::write(std::ofstream& writer) const
	{
		using namespace Format::File::Beatmap::Difficulty;

		writer << HEADER << '\n';
		writer << HP << Format::File::Beatmap::SEPARATOR << hp << '\n';
		writer << OD << Format::File::Beatmap::SEPARATOR << od << '\n';
		writer << VELOCITY << Format::File::Beatmap::SEPARATOR << velocity << '\n';
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
	int16_t CalculatedDifficulty::OverallDifficulty::get_score(
		const int64_t& click_moment, const int64_t& hit_object_time,
		const Types::Game::Direction::Direction& required_direction,
		const Types::Game::Direction::Direction& current_direction) const
	{
		if (required_direction == current_direction)
		{
			if (Utilities::Math::in_offset_range(static_cast<float>(hit_object_time), perfect, static_cast<float>(click_moment)))
				return 300;
			if (Utilities::Math::in_offset_range(static_cast<float>(hit_object_time), great, static_cast<float>(click_moment)))
				return 100;
			if (Utilities::Math::in_offset_range(static_cast<float>(hit_object_time), bad, static_cast<float>(click_moment)))
				return 50;
			if (Utilities::Math::in_offset_range(static_cast<float>(hit_object_time), miss, static_cast<float>(click_moment))
				|| static_cast<float>(click_moment) > static_cast<float>(hit_object_time) + miss)
				return 0;
		}
		else if (Utilities::Math::in_offset_range(static_cast<float>(hit_object_time), miss, static_cast<float>(click_moment))
			|| static_cast<float>(click_moment) > static_cast<float>(hit_object_time) + miss)
			return 0;
		return -1; // chưa tới tầm với
	}
	// ::HealthPoint
	void CalculatedDifficulty::HealthPoint::apply(const float& v)
	{
		value = v;
		using namespace Config::GameConfig::Difficulty::HP;

		if (value < 5) 
			drain = BASE - EASIER_MULTIPLY * (5 - value);
		else 
			drain = BASE + HARDER_MULTIPLY * (value - 5);
	}
	float CalculatedDifficulty::HealthPoint::get_note_hp(const int16_t& note_score, const unsigned long& current_combo) const
	{
		float note_multiply;
		switch (note_score)
		{
		case 100: note_multiply = 0.8f;	break;
		case 50: note_multiply = 0.5f;	break;
		case 0:	note_multiply = 0.0f;	break;
		default: // 300, slider tick
			note_multiply = 1.0f;
			break;
		}

		if (current_combo <= 5)
			return drain * note_multiply - drain;
		if (current_combo <= 10)
			return (drain * note_multiply - drain) + (drain * 0.2f * static_cast<float>(current_combo - 5) / 5);
		return (drain * note_multiply - drain) + (drain * 0.2f); // 0.2 => safe for 100 (0.8)
	}
	void CalculatedDifficulty::Velocity::apply(const float& v)
	{
		value = v;
		speed = v * Config::GameConfig::Difficulty::Velocity::BASE_SPEED;
	}
	// ::
	void CalculatedDifficulty::write(std::ofstream& writer) const
	{
		using namespace  Format::File::Beatmap::Difficulty;

		writer << HEADER << '\n';
		writer << HP << Format::File::Beatmap::SEPARATOR << hp.value << '\n';
		writer << OD << Format::File::Beatmap::SEPARATOR << od.value << '\n';
		writer << VELOCITY << Format::File::Beatmap::SEPARATOR << velocity.value << '\n';
		writer << '\n';
	}
	void CalculatedDifficulty::apply(const Difficulty& basic)
	{
		od.apply(basic.od);
		hp.apply(basic.hp);
		velocity.apply(basic.velocity);
	}
}