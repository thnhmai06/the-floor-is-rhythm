#include "convert/osu.h"
#include "game/beatmap.h"
#include <osu!parser/Parser.hpp>
#include "format.h"
#include <fstream>

using Parser::NA;

static General convert_general(const Parser::GeneralSection& general)
{
	General result;
	result.audio_file = general.AudioFilename;
	if (general.AudioLeadIn != NA) result.start_music_delay = std::stoi(general.AudioLeadIn);
	if (general.PreviewTime != NA) result.preview_timestamp = std::stoi(general.PreviewTime);
	if (general.EpilepsyWarning != NA) result.epilepsy_warning = (std::stoi(general.EpilepsyWarning) == 1);

	return result;
}
static Difficulty convert_difficulty(const Parser::DifficultySection& difficulty)
{
	Difficulty result;
	if (difficulty.ApproachRate != NA) result.ar = std::stof(difficulty.ApproachRate);
	if (difficulty.OverallDifficulty != NA) result.od = std::stof(difficulty.OverallDifficulty);
	if (difficulty.HPDrainRate != NA) result.hp = std::stof(difficulty.HPDrainRate);

	return result;
}
static Metadata convert_metadata(const Parser::MetadataSection& metadata)
{
	Metadata result;
	result.title = metadata.TitleUnicode;
	result.artist = metadata.ArtistUnicode;
	result.creator = metadata.Creator;
	result.difficulty_name = metadata.Version;
	result.source = metadata.Source;
	return result;
}

void convert(const char* file_name, const char* output)
{
	Parser::Beatmap beatmap(file_name);
	std::ofstream writer(output);
	if (!writer) { return; } //TODO: Exception: Không tạo được file

	// Version
	writer << tfir::FORMAT_VERSION << '\n';
	writer << tfir::CONVERSION::FORMAT_VERSION << '\n';
	writer << '\n';
	// Content
	convert_general(beatmap.General).print(writer);
	convert_difficulty(beatmap.Difficulty).print(writer);
	convert_metadata(beatmap.Metadata).print(writer);

	writer.close();
}