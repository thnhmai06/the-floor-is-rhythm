#include "file/osu/osu_beatmap.h" // Header
#include <osu!parser/Parser.hpp>
#include <fstream>
#include "rule/file_format.h"
#include "game/metadata.h"
#include "game/hitobject.h"
#include "game/timing.h"
#include "exceptions.h"
#include "logging.h"
#include "template.h"
#include "utilities.h"

using namespace GameObjects::Metadata;
static Template::Game::Direction::Rotation get_rotation(const uint8_t colour_hax)
{
	// Loại bỏ hai hướng là cùng phương là hướng hiện tại và hướng ngược với nó
	// Ta còn lại hai hướng phương vuông góc, colour_hax sẽ quyết định là hướng nào (vì còn 2 hướng nên quyết định bằng tính chẵn lẻ)
	// colour_hax: 1 -> 7 (3 cùng tính chẵn lẻ, 4 không cùng) nên xác suất là như nhau khi đầy đủ (trừ khi bên beatmap osu lẻ combo color)
	if (colour_hax == 0) return Template::Game::Direction::Rotation::NO_ROTATE;
	return Utilities::Math::is_bit_enabled(colour_hax, 1) ? Template::Game::Direction::Rotation::ROTATE_90 : Template::Game::Direction::Rotation::ROTATE_270;
}
static General convert_general(const Parser::GeneralSection& general)
{
	General result;
	if (!general.AudioFilename.empty()) result.audio_file = general.AudioFilename;
	if (!general.AudioLeadIn.empty()) result.start_music_delay = std::stoi(general.AudioLeadIn);
	if (!general.PreviewTime.empty()) result.preview_timestamp = std::stoi(general.PreviewTime);
	if (!general.EpilepsyWarning.empty()) result.epilepsy_warning = (std::stoi(general.EpilepsyWarning) == 1);
	return result;
}
static Difficulty convert_difficulty(const Parser::DifficultySection& difficulty)
{
	Difficulty result;
	if (!difficulty.OverallDifficulty.empty()) result.od = std::stof(difficulty.OverallDifficulty);
	if (!difficulty.HPDrainRate.empty()) result.hp = std::stof(difficulty.HPDrainRate);
	if (!difficulty.SliderMultiplier.empty()) result.velocity = std::stof(difficulty.SliderMultiplier);
	return result;
}
static Metadata convert_metadata(const Parser::MetadataSection& metadata)
{
	Metadata result;
	if (!metadata.Title.empty()) result.title = metadata.Title;
	if (!metadata.Artist.empty()) result.artist = metadata.Artist;
	if (!metadata.Creator.empty()) result.creator = metadata.Creator;
	if (!metadata.Version.empty()) result.difficulty_name = metadata.Version;
	if (!metadata.Source.empty()) { result.source = metadata.Source; result.source.push_back(' '); }
	result.source.append(FileFormat::Beatmap::CONVERT::osu::SOURCE);
	result.tags = metadata.Tags;
	return result;
}
//! HitObjects
using namespace GameObjects::HitObjects;
static Slider convert_hitobject_slider(const Parser::HitObject& object)
{
	Slider slider;
	slider.time = object.Time;
	slider.end_time = object.EndTime;
	slider.rotation = get_rotation(object.Type.ColourHax);
	slider.combo_jump = object.Type.ColourHax;
	slider.hitsound = object.Hitsound;
	slider.hitsample = object.Hitsample;
	return slider;
}
static Floor convert_hitobject_floor(const Parser::HitObject& object)
{
	Floor floor;
	floor.time = object.Time;
	floor.rotation = get_rotation(object.Type.ColourHax);
	floor.combo_jump = object.Type.ColourHax;
	floor.hitsound = object.Hitsound;
	floor.hitsample = object.Hitsample;
	return floor;
}
static HitObjects convert_hitobjects(const std::vector<Parser::HitObject>& objects)
{
	HitObjects result;
	for (const Parser::HitObject& osu_object : objects)
	{
		const auto back_itr = (result.empty()) ? (result.end()) : (std::prev(result.end()));
		if (osu_object.Type.HitCircle)
			result.emplace_hint(back_itr, osu_object.Time,
				std::make_unique<Floor>(convert_hitobject_floor(osu_object)));
		else result.emplace_hint(back_itr, osu_object.Time,
			std::make_unique<Slider>(convert_hitobject_slider(osu_object)));
	}
	return result;
}
//! TimingPoints
using namespace GameObjects::Timing;
using GameObjects::Hitsound::SampleSetType;
static UninheritedPoint convert_uninherited_point(const Parser::TimingPoint& timing_point)
{
	UninheritedPoint result;
	result.time = timing_point.Time;
	result.beat_length = static_cast<float>(timing_point.BeatLength);
	result.sample_set = static_cast<SampleSetType>(timing_point.SampleSet);
	result.sample_index = timing_point.SampleIndex;
	result.volume = timing_point.Volume;
	result.kiai = timing_point.Effects.kiai;
	return result;
}
static InheritedPoint convert_inherited_point(const Parser::TimingPoint& timing_point)
{
	InheritedPoint result;
	result.time = timing_point.Time;
	result.velocity = (100.0f / static_cast<float>(-timing_point.BeatLength));
	result.sample_set = static_cast<SampleSetType>(timing_point.SampleSet);
	result.sample_index = timing_point.SampleIndex;
	result.volume = timing_point.Volume;
	result.kiai = timing_point.Effects.kiai;
	return result;
}
static TimingPoints convert_timing_points(const std::vector<Parser::TimingPoint>& timing_points)
{
	TimingPoints result;
	for (const Parser::TimingPoint& timing_point : timing_points)
	{
		const auto back_itr = (result.empty()) ? (result.end()) : (std::prev(result.end()));
		if (timing_point.Uninherited)
			result.emplace_hint(back_itr, timing_point.Time,
				std::make_unique<UninheritedPoint>(convert_uninherited_point(timing_point)));
		else result.emplace_hint(back_itr, timing_point.Time,
			std::make_unique<InheritedPoint>(convert_inherited_point(timing_point)));
	}
	return result;
}

void convert_beatmap(const char* file_name, const char* output)
{
	Parser::Beatmap beatmap(file_name);
	std::ofstream writer(output);
	if (!writer)
		THROW_ERROR(FileExceptions::File_Open_Failed(output));
	// Version
	writer << FileFormat::Beatmap::FORMAT_VERSION << '\n';
	writer << FileFormat::Beatmap::CONVERT::osu::VERSION << beatmap.Version << '\n';
	writer << '\n';
	// Contents
	convert_general(beatmap.General).write(writer);
	convert_difficulty(beatmap.Difficulty).write(writer);
	convert_metadata(beatmap.Metadata).write(writer);
	convert_timing_points(beatmap.TimingPoints).write(writer);
	convert_hitobjects(beatmap.HitObjects).write(writer);

	writer.close();
}