#include "convert/osu.h"
constexpr std::string CONVERT_VER = "tfir file format v1";
constexpr std::string FILE_FORMAT_VER = "osu to tfir conversion v1";

void convert(const char* file_name, const char* output)
{
	std::ifstream reader;
	std::ofstream writer;
	reader.open(file_name);
	if (!reader.is_open()) goto end; // TODO: Exception: Cannot open file
	writer.open(output);
	if (!writer.is_open()) goto end; // TODO: Exception: Cannot open file

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
			if (header == "[General]") convert_general(reader, header);
			else if (header == "[Difficulty]") convert_difficulty(reader, header);
			else if (header == "[Metadata]") convert_metadata(reader, header);
			else break;
		} while (!reader.eof());
	} while (!reader.eof());

	end:
	reader.close();
	writer.close();
}

static General convert_general(std::ifstream& reader, std::string& header)
{
	/*AUDIO_FILENAME = "AudioFilename";
	AUDIO_LEADIN = "AudioLeadIn";
	PREVIEW_TIME = "PreviewTime";
	EPILEPSY_WARNING = "EpilepsyWarning";*/
}

static Difficulty convert_difficulty(std::ifstream& reader, std::string& header)
{
	/*const std::string AR = "ApproachRate";
	const std::string OD = "OverallDifficulty";
	const std::string HP = "HPDrainRate";*/
}

static Metadata convert_metadata(std::ifstream& reader, std::string& header)
{
	/*constexpr std::string TITLE = "TitleUnicode";
	const std::string ARTIST = "ArtistUnicode";
	const std::string CREATOR = "Creator";
	const std::string DIFF_NAME = "Version";
	const std::string SOURCE = "Source";
	const std::string TAGS = "Tags";*/

}