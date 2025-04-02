#include "file/tfir/tfir_beatmap.h" // Header
#include <fstream>
#include <unordered_map>
#include "rule/file_format.h"
#include "exceptions.h"
#include "logging.h"
#include "utilities.h"

static constexpr int32_t MINIMUM_LINE_CHARACTERS = 3;

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
		if (header == FileFormat::Beatmap::General::HEADER) beatmap.general.read(contents);
		else if (header == FileFormat::Beatmap::Metadata::HEADER)beatmap.metadata.read(contents);
		else if (header == FileFormat::Beatmap::Difficulty::HEADER) beatmap.calculated_difficulty.apply(Difficulty(contents));
		else if (header == FileFormat::Beatmap::TimingPoints::HEADER) beatmap.timing_points.read(contents);
		else if (header == FileFormat::Beatmap::HitObjects::HEADER) beatmap.hit_objects.read(contents);
	}
}

BeatmapFile::BeatmapFile(const char* file_path)
{
	std::ifstream reader(file_path);
	if (!reader)
		THROW_ERROR(FileExceptions::File_Open_Failed(file_path));

	parse_beatmap(*this, read_content(reader));
	reader.close();
}