#include "file/tfir/tfir_beatmap.h" // Header
#include <fstream>
#include <unordered_map>
#include "rule/file_format.h"
#include "exceptions.h"
#include "logging.h"
#include "utilities.h"

static constexpr int32_t MINIMUM_LINE_CHARACTERS = 3;
static Metadata::General parse_general(const std::vector<std::string>& contents)
{
	Metadata::General general;
	for (const auto& line : contents)
	{
		if (line.find(tfir::SEPARATOR) == std::string::npos) continue;
		const auto content = Utilities::String::split(line, tfir::SEPARATOR, true);
		if (content.size() <= 1) continue;

		// [0] là key, [1] là value
		if (content[0] == tfir::General::AUDIO_FILE)
			general.audio_file = content[1];
		else if (content[0] == tfir::General::MUSIC_DELAY)
			general.start_music_delay = std::stoi(content[1]);
		else if (content[0] == tfir::General::PREVIEW_TIMESTAMP)
			general.preview_timestamp = std::stoi(content[1]);
		else if (content[0] == tfir::General::EPILEPSY_WARNING)
			general.epilepsy_warning = std::stoi(content[1]);
	}
	return general;
}
static Metadata::Metadata parse_metadata(const std::vector<std::string>& contents)
{
	Metadata::Metadata metadata;
	for (const auto& line : contents)
	{
		if (line.find(tfir::SEPARATOR) == std::string::npos) continue;
		const auto content = Utilities::String::split(line, tfir::SEPARATOR, true);
		if (content.size() <= 1) continue;
		// [0] là key, [1] là value
		if (content[0] == tfir::Metadata::TITLE)
			metadata.title = content[1];
		else if (content[0] == tfir::Metadata::ARTIST)
			metadata.artist = content[1];
		else if (content[0] == tfir::Metadata::CREATOR)
			metadata.creator = content[1];
		else if (content[0] == tfir::Metadata::DIFF_NAME)
			metadata.difficulty_name = content[1];
		else if (content[0] == tfir::Metadata::SOURCE)
			metadata.source = content[1];
		else if (content[0] == tfir::Metadata::TAGS)
			metadata.tags = Utilities::String::split(content[1], ' ');
	}
	return metadata;
}
static Metadata::Difficulty parse_difficulty(const std::vector<std::string>& contents)
{
	Metadata::Difficulty difficulty;
	for (const auto& line : contents)
	{
		if (line.find(tfir::SEPARATOR) == std::string::npos) continue;
		const auto content = Utilities::String::split(line, tfir::SEPARATOR, true);
		if (content.size() <= 1) continue;
		// [0] là key, [1] là value
		if (content[0] == tfir::Difficulty::HP)
			difficulty.HP = std::stof(content[1]);
		else if (content[0] == tfir::Difficulty::OD)
			difficulty.OD = std::stof(content[1]);
		else if (content[0] == tfir::Difficulty::AR)
			difficulty.AR = std::stof(content[1]);
	}
	return difficulty;
}
static HitObjects parse_hitobjects(const std::vector<std::string>& contents)
{
	using namespace tfir::HitObjects;
	HitObjects objects;
	for (const auto& line : contents)
	{
		const auto content = Utilities::String::split(line, tfir::AND);
		if (const Type type = static_cast<Type>(std::stoi(content[3])); type == Type::FLOOR)
		{
			HitObject::Floor obj;
			obj.time = std::stoi(content[0]);
			obj.direction_jump = std::stoi(content[1]);
			obj.combo_jump = std::stoi(content[2]);
			obj.hitsound = Hitsound{ std::stoi(content[4]) };
			obj.hitsample = HitSample{ content[5] };

			auto back_itr = objects.empty() ? objects.end() : std::prev(objects.end());
			objects.emplace_hint(back_itr, obj.time, obj);
		} else if (type == Type::SLIDER)
		{
			HitObject::Slider obj;
			obj.time = std::stoi(content[0]);
			obj.direction_jump = std::stoi(content[1]);
			obj.combo_jump = std::stoi(content[2]);
			obj.end_time = std::stoi(content[4]);

			// Curves
			for (const auto curves = Utilities::String::split(content[5], Slider::AND); const auto& curve : curves)
			{
				// Curve
				const auto curve_content = Utilities::String::split(curve, Slider::CURVE_AND, true);
				obj.curves.push_back({
					.add_time= std::stoi(curve_content[0]), .direction_jump= static_cast<uint8_t>(std::stoi(curve_content[1]))
				});
			}
			obj.hitsound = Hitsound{ std::stoi(content[6]) };
			obj.hitsample = HitSample{ content[7] };
		}
	}
	return objects;
}
static std::unordered_map<std::string, std::vector<std::string>> read_content(std::ifstream& reader)
{
    std::unordered_map<std::string, std::vector<std::string>> sections;

    std::string current_line;
    std::getline(reader, current_line);
    while (std::getline(reader, current_line))
    {
        current_line = Utilities::String::trim(current_line);
        static std::string current_section;

        // Nếu current_line là một Header của section
        if (!current_line.empty() && current_line.front() == '[' && current_line.back() == ']')
			current_section = Utilities::String::trim(current_line);
        // Bỏ qua những line không đủ kí tự (điều kiện cần)
        else if (current_line.size() < MINIMUM_LINE_CHARACTERS) {}
        else
            // Những line còn lại là content, sẽ được thêm vào section hiện tại
            sections[current_section].push_back(current_line);
    }
    return sections;
}
static void parse_beatmap(BeatmapFile& beatmap, const std::unordered_map<std::string, std::vector<std::string>>& sections)
{
	for (const auto& [header, contents] : sections)
	{
		if (header == tfir::General::HEADER)
			beatmap.general = parse_general(contents);
		else if (header == tfir::Metadata::HEADER)
			beatmap.metadata = parse_metadata(contents);
		else if (header == tfir::Difficulty::HEADER)
			beatmap.calculated_difficulty = Metadata::CalculatedDifficulty(parse_difficulty(contents));
		else if (header == tfir::HitObjects::HEADER)
			beatmap.hit_objects = parse_hitobjects(contents);
	}
}

BeatmapFile::BeatmapFile(const char* file_path)
{
	std::ifstream reader(file_path);
	if (!reader)
		THROW_ERROR(File_Exceptions::File_Open_Failed(file_path));
    parse_beatmap(*this, read_content(reader));
	reader.close();
}