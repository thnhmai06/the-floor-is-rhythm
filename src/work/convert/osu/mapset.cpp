#include "work/convert/osu/mapset.h" // Header
#include <osu!parser/Parser/Beatmap.hpp>
#include "format/file.h"
#include "structures/game/beatmap/metadata.h"
#include "structures/game/beatmap/hitobject.h"
#include "structures/game/beatmap/timing.h"
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "structures/types.h"
#include "utilities.hpp"

namespace Work::Convert::osu
{
	using namespace Structures::Game::Beatmap::Metadata;
	using namespace OsuParser::Beatmap;

	static uint8_t get_current_combo_colour(const uint8_t& previous_colour, const bool new_combo, const uint8_t& colour_hax)
	{
		return (previous_colour + new_combo + colour_hax) % Structures::Game::Beatmap::HitObjects::COLOURS_NUM;
	}

	//! Metadata
	static General convert_general(const Sections::General::GeneralSection& general)
	{
		General result;
		if (!general.AudioFilename.empty()) result.audio_file = general.AudioFilename;
		if (!general.AudioLeadIn.empty()) result.start_music_delay = std::stoi(general.AudioLeadIn);
		if (!general.PreviewTime.empty()) result.preview_timestamp = std::stoi(general.PreviewTime);
		if (!general.EpilepsyWarning.empty()) result.epilepsy_warning = (std::stoi(general.EpilepsyWarning) == 1);
		return result;
	}
	static Difficulty convert_difficulty(const Sections::Difficulty::DifficultySection& difficulty)
	{
		Difficulty result;
		if (!difficulty.OverallDifficulty.empty()) result.od = std::stof(difficulty.OverallDifficulty);
		if (!difficulty.HPDrainRate.empty()) result.hp = std::stof(difficulty.HPDrainRate);
		if (!difficulty.SliderMultiplier.empty()) result.vl = std::stof(difficulty.SliderMultiplier);
		return result;
	}
	static Metadata convert_metadata(const Sections::Metadata::MetadataSection& metadata)
	{
		Metadata result;
		if (!metadata.Title.empty()) result.title = metadata.Title;
		if (!metadata.Artist.empty()) result.artist = metadata.Artist;
		if (!metadata.Creator.empty()) result.creator = metadata.Creator;
		if (!metadata.Version.empty()) result.difficulty_name = metadata.Version;
		if (!metadata.Source.empty()) { result.source = metadata.Source; result.source.push_back(' '); }
		result.source.append(Format::File::Floor::Mapset::ConvertInformation::osu::SOURCE);
		result.tags = metadata.Tags;
		return result;
	}
	//! TimingPoints
	using namespace Structures::Game::Beatmap::TimingPoints;
	using Structures::Game::Beatmap::Hitsound::HitSampleType;
	static TimingPoint convert_timing_point(const Objects::TimingPoint::TimingPoint& timing_point)
	{
		TimingPoint result;
		result.time = timing_point.Time;
		result.beat_length = static_cast<float>(timing_point.BeatLength);
		result.sample_set = static_cast<HitSampleType>(timing_point.SampleSet);
		result.sample_index = timing_point.SampleIndex;
		result.volume = timing_point.Volume;
		result.kiai = timing_point.Effects.kiai;
		return result;
	}
	static TimingPoints convert_timing_points(const std::vector<Objects::TimingPoint::TimingPoint>& timing_points)
	{
		TimingPoints result;
		for (const auto timing_point : timing_points)
		{
			const auto back_itr = Utilities::Container::get_last_element_iterator(result.data);
			result.data.emplace_hint(back_itr, timing_point.Time, convert_timing_point(timing_point));
		}
		return result;
	}
	//! HitObjects
	using namespace Structures::Game::Beatmap::HitObjects;
	static Floor convert_hit_object_floor(const Objects::HitObject::HitObject& object, const uint8_t& previous_combo_colour)
	{
		Floor floor;
		floor.time = object.Time;
		floor.type_data.combo_colour = get_current_combo_colour(previous_combo_colour, object.Type.IsNewCombo, object.Type.ColourHax);
		floor.hit_sound = object.Hitsound;
		floor.hit_sample = object.Hitsample;
		return floor;
	}
	static Slider convert_hit_object_slider(const Objects::HitObject::HitObject& object, const uint8_t& previous_combo_colour)
	{
		Slider slider;
		slider.end_time = slider.time = object.Time;
		if (object.EndTime.has_value()) 
			slider.end_time = object.EndTime.value();
		slider.type_data.combo_colour = get_current_combo_colour(previous_combo_colour, object.Type.IsNewCombo, object.Type.ColourHax);
		slider.hit_sound = object.Hitsound;
		slider.hit_sample = object.Hitsample;
		for (const auto& [sound, sample] : object.SliderParameters->edgeHitsounds)
		{
			slider.edge_sounds.emplace_back(sound.ToInt());
			slider.edge_sets.emplace_back(sample);
		}
		return slider;
	}
	static HitObjects convert_hit_objects(const std::vector<Objects::HitObject::HitObject>& objects)
	{
		HitObjects result;
		uint8_t previous_combo_colour = 0;
		for (const auto& osu_object : objects)
		{
			const auto last_itr = Utilities::Container::get_last_element_iterator(result.data);
			if (osu_object.Type.HitCircle)
			{
				result.data.emplace_hint(last_itr, osu_object.Time, 
					std::make_shared<Floor>(convert_hit_object_floor(osu_object, previous_combo_colour)));
				previous_combo_colour = result.data.rbegin()->second->type_data.combo_colour;
			}
			else // slider, spinner...
			{
				result.data.emplace_hint(last_itr, osu_object.Time,
					std::make_shared<Slider>(convert_hit_object_slider(osu_object, previous_combo_colour)));
				previous_combo_colour = result.data.rbegin()->second->type_data.combo_colour;
			}
		}
		return result;
	}

	void convert_mapset(const std::filesystem::path& file, const std::filesystem::path& output)
	{
		const Beatmap beatmap(file.string());
		std::ofstream writer(output);
		if (!writer)
			THROW_ERROR(Logging::Exceptions::FileExceptions::File_Open_Failed(output));

		writer << Format::File::Floor::Mapset::FORMAT_VERSION << '\n';
		writer << Format::File::Floor::Mapset::ConvertInformation::osu::VERSION << beatmap.Version << "\n\n";

		writer << convert_general(beatmap.General) << "\n\n";
		writer << convert_difficulty(beatmap.Difficulty) << "\n\n";
		writer << convert_metadata(beatmap.Metadata) << "\n\n";
		writer << convert_timing_points(beatmap.TimingPoints) << "\n\n";
		writer << convert_hit_objects(beatmap.HitObjects) << "\n\n";
		writer << beatmap.Events << '\n';
		writer.close();
	}
}