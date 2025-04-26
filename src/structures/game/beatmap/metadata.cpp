#include "structures/game/beatmap/metadata.h" // Header
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
			if (line.find(Format::File::Floor::Mapset::SEPARATOR) == std::string::npos) continue;
			const auto content = Utilities::String::split(line, Format::File::Floor::Mapset::SEPARATOR, true);
			if (content.size() <= 1) continue;

			// [0] là key, [1] là value
			if (content[0] == Format::File::Floor::Mapset::General::AUDIO_FILE)
				audio_file = content[1];
			else if (content[0] == Format::File::Floor::Mapset::General::MUSIC_DELAY)
				start_music_delay = std::stoi(content[1]);
			else if (content[0] == Format::File::Floor::Mapset::General::PREVIEW_TIMESTAMP)
				preview_timestamp = std::stoi(content[1]);
			else if (content[0] == Format::File::Floor::Mapset::General::EPILEPSY_WARNING)
				epilepsy_warning = std::stoi(content[1]);
		}
	}
	std::string General::to_string() const
	{
		using namespace Format::File::Floor::Mapset::General;

		std::stringstream ss;
		ss << HEADER << '\n';
		ss << AUDIO_FILE << Format::File::Floor::Mapset::SEPARATOR << audio_file << '\n';
		ss << MUSIC_DELAY << Format::File::Floor::Mapset::SEPARATOR << start_music_delay << '\n';
		ss << PREVIEW_TIMESTAMP << Format::File::Floor::Mapset::SEPARATOR << preview_timestamp << '\n';
		ss << EPILEPSY_WARNING << Format::File::Floor::Mapset::SEPARATOR << epilepsy_warning;

		return ss.str();
	}
	std::ostream& operator<<(std::ostream& os, const General& general)
	{
		os << general.to_string();
		return os;
	}

	//! Metadata::Metadata
	void Metadata::read(const std::vector<std::string>& contents)
	{
		for (const auto& line : contents)
		{
			if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
			if (line.find(Format::File::Floor::Mapset::SEPARATOR) == std::string::npos) continue;
			const auto content = Utilities::String::split(line, Format::File::Floor::Mapset::SEPARATOR, true);
			if (content.size() <= 1) continue;
			// [0] là key, [1] là value
			if (content[0] == Format::File::Floor::Mapset::Metadata::TITLE)
				title = content[1];
			else if (content[0] == Format::File::Floor::Mapset::Metadata::ARTIST)
				artist = content[1];
			else if (content[0] == Format::File::Floor::Mapset::Metadata::CREATOR)
				creator = content[1];
			else if (content[0] == Format::File::Floor::Mapset::Metadata::DIFF_NAME)
				difficulty_name = content[1];
			else if (content[0] == Format::File::Floor::Mapset::Metadata::SOURCE)
				source = content[1];
			else if (content[0] == Format::File::Floor::Mapset::Metadata::TAGS)
				tags = Utilities::String::split(content[1], ' ');
		}
	}
	std::string Metadata::to_string() const
	{
		using namespace Format::File::Floor::Mapset::Metadata;

		// Chuyển unordered_set thành string
		std::string tags_str;
		for (auto& tag : tags)
		{
			if (!tags_str.empty()) tags_str.push_back(' ');
			tags_str.append(tag);
		}

		std::stringstream writer;
		writer << HEADER << '\n';
		writer << TITLE << Format::File::Floor::Mapset::SEPARATOR << title << '\n';
		writer << ARTIST << Format::File::Floor::Mapset::SEPARATOR << artist << '\n';
		writer << CREATOR << Format::File::Floor::Mapset::SEPARATOR << creator << '\n';
		writer << DIFF_NAME << Format::File::Floor::Mapset::SEPARATOR << difficulty_name << '\n';
		writer << SOURCE << Format::File::Floor::Mapset::SEPARATOR << source << '\n';
		writer << TAGS << Format::File::Floor::Mapset::SEPARATOR << tags_str;

		return writer.str();
	}
	std::ostream& operator<<(std::ostream& os, const Metadata& metadata)
	{
		os << metadata.to_string();
		return os;
	}

	//! Metadata::Difficulty
	void Difficulty::read(const std::vector<std::string>& contents)
	{
		for (const auto& line : contents)
		{
			if (line.size() < MINIMUM_LINE_CHARACTERS) continue;
			if (line.find(Format::File::Floor::Mapset::SEPARATOR) == std::string::npos) continue;
			const auto content = Utilities::String::split(line, Format::File::Floor::Mapset::SEPARATOR, true);
			if (content.size() <= 1) continue;
			// [0] là key, [1] là value
			if (content[0] == Format::File::Floor::Mapset::Difficulty::HP)
				hp = std::stof(content[1]);
			else if (content[0] == Format::File::Floor::Mapset::Difficulty::OD)
				od = std::stof(content[1]);
			else if (content[0] == Format::File::Floor::Mapset::Difficulty::VELOCITY)
				vl = std::stof(content[1]);
		}
	}
	std::string Difficulty::to_string() const
	{
		using namespace Format::File::Floor::Mapset::Difficulty;

		std::stringstream writer;
		writer << HEADER << '\n';
		writer << HP << Format::File::Floor::Mapset::SEPARATOR << hp << '\n';
		writer << OD << Format::File::Floor::Mapset::SEPARATOR << od << '\n';
		writer << VELOCITY << Format::File::Floor::Mapset::SEPARATOR << vl;

		return writer.str();
	}
	CalculatedDifficulty Difficulty::calculate() const
	{
		return CalculatedDifficulty(*this);
	}
	std::ostream& operator<<(std::ostream& os, const Difficulty& difficulty)
	{
		os << difficulty.to_string();
		return os;
	}

	//! Metadata::CalculatedDifficulty
	// ::OverallDifficulty
	void CalculatedDifficulty::OverallDifficulty::link(const float* od) : value(od)
	{
	}
	float CalculatedDifficulty::OverallDifficulty::get_perfect() const
	{
		return Config::GameConfig::Difficulty::OD::Base::PERFECT - *value * Config::GameConfig::Difficulty::OD::Multiply::PERFECT;
	}
	float CalculatedDifficulty::OverallDifficulty::get_good() const
	{
		return Config::GameConfig::Difficulty::OD::Base::GOOD - *value * Config::GameConfig::Difficulty::OD::Multiply::GOOD;
	}
	float CalculatedDifficulty::OverallDifficulty::get_bad() const
	{
		return Config::GameConfig::Difficulty::OD::Base::BAD - *value * Config::GameConfig::Difficulty::OD::Multiply::BAD;
	}
	float CalculatedDifficulty::OverallDifficulty::get_miss()
	{
		return Config::GameConfig::Difficulty::OD::Base::MISS;
	}
	int16_t CalculatedDifficulty::OverallDifficulty::get_score(
		const bool is_clicked, const float& click_moment, const int64_t& hit_object_time) const
	{
		const auto perfect = get_perfect();
		const auto good = get_good();
		const auto bad = get_bad();
		constexpr auto miss = get_miss();

		if (click_moment > static_cast<float>(hit_object_time) + bad) return 0; // quá muộn
		if (click_moment < static_cast<float>(hit_object_time) - miss) return -1; // chưa đến
		if (!is_clicked) return -1;

		// Nếu bấm
		if (Utilities::Math::in_offset_range(static_cast<float>(hit_object_time), perfect, click_moment))
			return 300;
		if (Utilities::Math::in_offset_range(static_cast<float>(hit_object_time), good, click_moment))
			return 100;
		if (Utilities::Math::in_offset_range(static_cast<float>(hit_object_time), bad, click_moment))
			return 50;
		return 0;
	}
	CalculatedDifficulty::OverallDifficulty::OverallDifficulty(const float* value) { link(value); }

	// ::HPDrainRate
	void CalculatedDifficulty::HPDrainRate::link(const float* hp) : value(hp)
	{
	}
	float CalculatedDifficulty::HPDrainRate::get_drain_rate() const
	{
		using namespace Config::GameConfig::Difficulty::HP;
		if (*value < 5)
			return BASE - EASIER_MULTIPLY * (5 - *value);
		return BASE + HARDER_MULTIPLY * (*value - 5);
	}
	float CalculatedDifficulty::HPDrainRate::get_gained_health(const int16_t& note_score, const unsigned long& current_combo) const
	{
		const auto drain = get_drain_rate();

		float note_multiply = 1.0f; // 300, slider tick
		switch (note_score)
		{
		case 100: note_multiply = 0.8f;	break;
		case 50: note_multiply = 0.5f;	break;
		case 0:	note_multiply = 0.0f;	break;
		default:						break;
		}

		if (current_combo <= 5) return drain * note_multiply - drain;
		if (current_combo <= 10) return (drain * note_multiply - drain) + (drain * 0.2f * static_cast<float>(current_combo - 5) / 5);
		return (drain * note_multiply - drain) + (drain * 0.2f); // 0.2 => safe for 100 (0.8)
	}

	//::Velocity
	void CalculatedDifficulty::Velocity::link(const float* vl) : value(vl)
	{
	}
	float CalculatedDifficulty::Velocity::get_pixel_speed() const
	{
		return *value * Config::GameConfig::Difficulty::Velocity::BASE_PIXEL_SPEED;
	}
}