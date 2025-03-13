#include "convert/osu.h"
#include "game/beatmap.h"
#include "format.h"
#include <fstream>

static General convert_general(std::ifstream& reader, std::string& header)
{
	using namespace	osu::General;

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
		size_t space_pos = line.find(osu::SPACING);
		if (space_pos != std::string::npos) // Tìm thấy dấu :
		{
			std::string key = line.substr(0, space_pos);
			std::string value = line.substr(space_pos + 1);

			if (key == AUDIO_FILENAME) data.audio_file = value;
			else if (key == AUDIO_LEADIN) data.start_music_delay = std::stoi(value);
			else if (key == PREVIEW_TIME) data.preview_timestamp = std::stoi(value);
			else if (key == EPILEPSY_WARNING) data.epilepsy_warning = (std::stoi(value) == 1);
		}
	}
	return data;
}
static Metadata convert_metadata(std::ifstream& reader, std::string& header)
{
	Metadata data;
	std::string line;

	using namespace osu::Metadata;

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
		size_t space_pos = line.find(osu::SPACING);
		if (space_pos != std::string::npos) // Tìm thấy dấu :
		{
			std::string key = line.substr(0, space_pos);
			std::string value = line.substr(space_pos + 1);

			if (key == TITLE) data.title = value;
			else if (key == ARTIST) data.artist = value;
			else if (key == CREATOR) data.creator = value;
			else if (key == DIFF_NAME) data.difficulty_name = value;
			else if (key == SOURCE) data.source = value;
			else if (key == TAGS)
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
static BasicDifficulty convert_difficulty(std::ifstream& reader, std::string& header)
{
	using namespace osu::Difficulty;

	BasicDifficulty data;
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
		size_t space_pos = line.find(osu::SPACING);
		if (space_pos != std::string::npos) // Tìm thấy dấu :
		{
			std::string key = line.substr(0, space_pos);
			std::string value = line.substr(space_pos + 1);
			if (key == AR) data.ar = std::stof(value);
			else if (key == OD) data.od = std::stof(value);
			else if (key == HP) data.hp = std::stof(value);
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
	writer << tlir::FORMAT_VERSION << '\n';
	writer << tlir::CONVERSION::FORMAT_VERSION << '\n';
	writer << '\n';

	// content
	std::string header;
	do
	{
		std::getline(reader, header, '\n');
		do {
			if (header == osu::General::HEADER) convert_general(reader, header).print(writer);
			else if (header == osu::Difficulty::HEADER) convert_difficulty(reader, header);
			else if (header == osu::Metadata::HEADER) convert_metadata(reader, header).print(writer);
			else break;
		} while (!reader.eof());
	} while (!reader.eof());

	close_file(reader, writer);
}