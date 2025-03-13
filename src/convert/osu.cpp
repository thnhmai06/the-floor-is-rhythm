#include "convert/osu.h"

#include <execution>
const std::string_view CONVERT_VER = "osu to tfir conversion v1";
const std::string_view FILE_FORMAT_VER = "tfir file format v1";
const char OSU_SPACING = ':';

static General convert_general(std::ifstream& reader, std::string& header)
{
	const std::string_view OSU_AUDIO_FILENAME = "AudioFilename";
	const std::string_view OSU_AUDIO_LEADIN = "AudioLeadIn";
	const std::string_view OSU_PREVIEW_TIME = "PreviewTime";
	const std::string_view OSU_EPILEPSY_WARNING = "EpilepsyWarning";

	General data;
	std::string line;

	while (!reader.eof())
	{
		getline(reader, line, '\n');

		// Kiểm tra line có phải Header mục khác không
		if (line.empty() || (line.front() == '[' && line.back() == ']'))
		{
			header = line;
			break;
		}

		// Lấy một line dữ liệu và xử lý
		size_t space_pos = line.find(OSU_SPACING);
		if (space_pos != std::string::npos) // Tìm thấy dấu :
		{
			std::string key = line.substr(0, space_pos);
			std::string value = line.substr(space_pos + 1);

			if (key == OSU_AUDIO_FILENAME) data.audio_file = value;
			else if (key == OSU_AUDIO_LEADIN) data.start_music_delay = std::stoi(value);
			else if (key == OSU_PREVIEW_TIME) data.preview_timestamp = std::stoi(value);
			else if (key == OSU_EPILEPSY_WARNING) data.epilepsy_warning = (std::stoi(value) == 1);
		}
	}
	return data;
}
static Metadata convert_metadata(std::ifstream& reader, std::string& header)
{
	const std::string_view OSU_TITLE = "Title";
	const std::string_view OSU_ARTIST = "Artist";
	const std::string_view OSU_CREATOR = "Creator";
	const std::string_view OSU_DIFF_NAME = "Version";
	const std::string_view OSU_SOURCE = "Source";
	const std::string_view OSU_TAGS = "Tags";

	Metadata data;
	std::string line;

	while (!reader.eof())
	{
		getline(reader, line, '\n');

		// Kiểm tra line có phải Header mục khác không
		if (line.empty() || (line.front() == '[' && line.back() == ']'))
		{
			header = line;
			break;
		}

		// Lấy một line dữ liệu và xử lý
		size_t space_pos = line.find(OSU_SPACING);
		if (space_pos != std::string::npos) // Tìm thấy dấu :
		{
			std::string key = line.substr(0, space_pos);
			std::string value = line.substr(space_pos + 1);

			if (key == OSU_TITLE) data.title = value;
			else if (key == OSU_ARTIST) data.artist = value;
			else if (key == OSU_CREATOR) data.creator = value;
			else if (key == OSU_DIFF_NAME) data.difficulty_name = value;
			else if (key == OSU_SOURCE) data.source = value;
			else if (key == OSU_TAGS)
			{
				std::string tags;
				for (char &i : value)
				{
					if (i == ' ') // Phân cách giữa 2 tag
					{
						data.tags.insert(tags);
						tags.clear();
					}
					else tags.push_back(i);
				}
				if (!tags.empty()) data.tags.insert(tags); // Tag cuối cùng
			}
		}
	}
	return data;
}
static Difficulty convert_difficulty(std::ifstream& reader, std::string& header)
{
	const std::string_view OSU_AR = "ApproachRate";
	const std::string_view OSU_OD = "OverallDifficulty";
	const std::string_view OSU_HP = "HPDrainRate";

	Difficulty data;
	std::string line;
	do
	{
		getline(reader, line, '\n');

		// Kiểm tra line có phải Header mục khác không
		if (line.empty() || (line.front() == '[' && line.back() == ']'))
		{
			header = line;
			break;
		}

		// Lấy một line dữ liệu và xử lý
		size_t space_pos = line.find(OSU_SPACING);
		if (space_pos != std::string::npos) // Tìm thấy dấu :
		{
			std::string key = line.substr(0, space_pos);
			std::string value = line.substr(space_pos + 1);
			if (key == OSU_AR) data.ar.value = std::stof(value);
			else if (key == OSU_OD) data.od.value = std::stof(value);
			else if (key == OSU_HP) data.hp.value = std::stof(value);
		}
	} while (!reader.eof());
	return data;
}

static void close_file(std::ifstream& reader, std::ofstream& writer)
{
	reader.close();
	writer.close();
}

void convert(const char* file_name, const char* output)
{
	std::ifstream reader;
	std::ofstream writer;
	reader.open(file_name);
	if (!reader) {
		// TODO: Exception: Cannot open file
		close_file(reader, writer); return;
	}
	writer.open(output);
	if (!writer) {
		// TODO: Exception: Cannot open file
		close_file(reader, writer); return;
	}
	// version
	writer << FILE_FORMAT_VER << '\n';
	writer << CONVERT_VER << '\n';
	writer << '\n';

	// content
	std::string header;
	do
	{
		std::getline(reader, header, '\n');
		do {
			if (header == "[General]") convert_general(reader, header).print(writer);
			else if (header == "[Difficulty]") convert_difficulty(reader, header);
			else if (header == "[Metadata]") convert_metadata(reader, header).print(writer);
			else break;
		} while (!reader.eof());
	} while (!reader.eof());

	close_file(reader, writer);
}