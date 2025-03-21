#include "convert/osu/beatmap.h" // Header
#include <osu!parser/Parser.hpp>
#include <fstream>
#include "game/metadata.h"
#include "game/object.h"
#include "utilities.h"
#include "rule/file_format.h"

static uint8_t get_direction_jump(const uint8_t colour_hax)
{
	constexpr uint8_t NO_ROTATE = 0;
	constexpr uint8_t ROTATE_90 = 1;
	constexpr uint8_t ROTATE_270 = 3;

	// Loại bỏ hai hướng là cùng phương là hướng hiện tại và hướng ngược với nó
	// Ta còn lại hai hướng phương vuông góc, colour_hax sẽ quyết định là hướng nào (vì còn 2 hướng nên quyết định bằng tính chẵn lẻ)
	// colour_hax: 1 -> 7 (3 cùng tính chẵn lẻ, 4 không cùng) nên xác suất là như nhau khi đầy đủ (trừ khi bên beatmap osu lẻ combo color)
	if (colour_hax == 0) return NO_ROTATE;
	return Utilities::Math::is_bit_enabled(colour_hax, 1) ? ROTATE_90 : ROTATE_270;
}
static Metadata::General convert_general(const Parser::GeneralSection& general)
{
	Metadata::General result;
	if (!general.AudioFilename.empty()) result.audio_file = general.AudioFilename;
	if (!general.AudioLeadIn.empty()) result.start_music_delay = std::stoi(general.AudioLeadIn);
	if (!general.PreviewTime.empty()) result.preview_timestamp = std::stoi(general.PreviewTime);
	if (!general.EpilepsyWarning.empty()) result.epilepsy_warning = (std::stoi(general.EpilepsyWarning) == 1);
	return result;
}
static Metadata::Difficulty convert_difficulty(const Parser::DifficultySection& difficulty)
{
	Metadata::Difficulty result;
	if (!difficulty.ApproachRate.empty()) result.AR = std::stof(difficulty.ApproachRate);
	if (!difficulty.OverallDifficulty.empty()) result.OD = std::stof(difficulty.OverallDifficulty);
	if (!difficulty.HPDrainRate.empty()) result.HP = std::stof(difficulty.HPDrainRate);
	return result;
}
static Metadata::Metadata convert_metadata(const Parser::MetadataSection& metadata)
{
	Metadata::Metadata result;
	if (!metadata.Title.empty()) result.title = metadata.Title;
	if (!metadata.Artist.empty()) result.artist = metadata.Artist;
	if (!metadata.Creator.empty()) result.creator = metadata.Creator;
	if (!metadata.Version.empty()) result.difficulty_name = metadata.Version;
	if (!metadata.Source.empty()) { result.source = metadata.Source; result.source.push_back(' '); }
	result.source.append(tfir::CONVERT::osu::SOURCE);
	result.tags = metadata.Tags;
	return result;
}
static HitObjects::Slider convert_hitobject_slider(const Parser::HitObject& object)
{
	HitObjects::Slider slider;
	slider.time = object.Time;
	slider.end_time = object.EndTime;
	slider.direction_jump = get_direction_jump(object.Type.ColourHax);
	slider.combo_jump = object.Type.ColourHax;
	slider.hitsound = object.Hitsound;
	slider.hitsample = object.Hitsample;
	return slider;
}
static HitObjects::Floor convert_hitobject_floor(const Parser::HitObject& object)
{
	HitObjects::Floor floor;
	floor.time = object.Time;
	floor.direction_jump = get_direction_jump(object.Type.ColourHax);
	floor.combo_jump = object.Type.ColourHax;
	floor.hitsound = object.Hitsound;
	floor.hitsample = object.Hitsample;
	return floor;
}

void convert(const char* file_name, const char* output)
{
	Parser::Beatmap beatmap(file_name);
	std::ofstream writer(output);
	if (!writer) { return; } //TODO: Exception: Không tạo được file

	// Version
	writer << tfir::FORMAT_VERSION << '\n';
	writer << tfir::CONVERT::osu::VERSION << beatmap.Version << '\n';
	writer << '\n';
	// Metadata
	convert_general(beatmap.General).write(writer);
	convert_difficulty(beatmap.Difficulty).write(writer);
	convert_metadata(beatmap.Metadata).write(writer);

	// HitObjects
	writer << tfir::HitObjects::HEADER << '\n';
	for (Parser::HitObject& object : beatmap.HitObjects)
	{
		if (object.Type.HitCircle) 
			convert_hitobject_floor(object).write(writer);
		else convert_hitobject_slider(object).write(writer);
	}
	writer << tfir::SECTION_END << '\n';

	writer.close();
}