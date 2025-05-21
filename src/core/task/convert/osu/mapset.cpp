#include "core/task/convert/osu/mapset.h" // Header
#include <osu!parser/Parser/Beatmap.hpp>
#include "format/file.h"
#include "structures/game/mapset/metadata.h"
#include "structures/game/mapset/hitobject.h"
#include "structures/game/mapset/timing.h"
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "core/type.h"
#include "utilities.h"

namespace Core::Task::Convert::Osu
{
	using namespace Structures::Game::Beatmap::Metadata;
	using namespace OsuParser::Beatmap;

	//! Metadata
	static General convert_general(const Sections::General::GeneralSection& general)
	{
		General result;
		result.audio_file = general.AudioFilename;
		result.start_music_delay = general.AudioLeadIn;
		result.preview_timestamp = general.PreviewTime;
		result.epilepsy_warning = general.EpilepsyWarning;
		result.widescreen_storyboard = general.WidescreenStoryboard;
		switch (general.SampleSet)
		{
		case Objects::TimingPoint::SampleSet::NO_CUSTOM:
			result.sample_set = SampleSet::NoCustom;
			break;
		case Objects::TimingPoint::SampleSet::NORMAL:
			result.sample_set = SampleSet::Normal;
			break;
		case Objects::TimingPoint::SampleSet::SOFT:
			result.sample_set = SampleSet::Soft;
			break;
		case Objects::TimingPoint::SampleSet::DRUM:
			result.sample_set = SampleSet::Drum;
			break;
		}

		return result;
	}
	static Difficulty convert_difficulty(const Sections::Difficulty::DifficultySection& difficulty)
	{
		Difficulty result;
		result.od = difficulty.OverallDifficulty;
		result.hp = difficulty.HPDrainRate;
		result.vc = difficulty.SliderMultiplier;
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
	using Structures::Game::Beatmap::Hitsound::SampleSet;
	static TimingPoint convert_timing_point(const Objects::TimingPoint::TimingPoint& timing_point)
	{
		TimingPoint result;
		result.time = timing_point.Time;
		result.beat_length = static_cast<float>(timing_point.BeatLength);
		result.sample.set = static_cast<SampleSet>(timing_point.SampleSet);
		result.sample.index = timing_point.SampleIndex;
		result.sample.volume.percent = static_cast<double>(timing_point.Volume);
		result.kiai = timing_point.Effects.kiai;
		return result;
	}
	static TimingPoints convert_timing_points(const Objects::TimingPoint::TimingPoints& timing_points)
	{
		TimingPoints result;
		for (const auto& timing_point : timing_points.data)
		{
			const auto back_itr = Utilities::Container::get_last_element_iterator(result.data);
			result.data.emplace_hint(back_itr, timing_point.Time, convert_timing_point(timing_point));
		}
		return result;
	}
	//! HitObjects
	using namespace Structures::Game::Beatmap::HitObjects;
	static std::vector<Floor> convert_hit_object(
		const Objects::HitObject::HitObject& object,
		bool& current_from_right)
	{
		std::vector<Floor> res{};
		if (!object.type.Slider && !object.type.HitCircle) return res;

		if (object.type.HitCircle)
		{
			Floor floor;
			floor.time = object.Time;
			if (object.type.IsNewCombo)
				current_from_right = !current_from_right;
			floor.is_kat = current_from_right;
			floor.additions = object.Hitsound;
			floor.hit_sample = object.Hitsample;
			res.push_back(std::move(floor));

			return res;
		}
		if (object.type.Slider)
		{
			const double one_slide_time = (object.EndTime.value() - object.Time) / object.SliderParameters.value().Slides;
			double current_time = object.Time;
			for (int32_t slide = 0; slide <= object.SliderParameters.value().Slides; ++slide, current_time += one_slide_time)
			{
				using Objects::TimingPoint::SampleSet;

				Floor floor;
				floor.time = static_cast<int64_t>(std::round(current_time));
				if ((slide == 0 && object.type.IsNewCombo) || slide >= 1)
					current_from_right = !current_from_right;
				floor.is_kat = current_from_right;
				floor.additions = object.SliderParameters.value().edgeSounds[slide];
				floor.hit_sample = object.Hitsample;
				floor.hit_sample = object.SliderParameters.value().edgeSets[slide];
				res.push_back(std::move(floor));
			}
		}
		return res;
	}
	static HitObjects convert_hit_objects(const Objects::HitObject::HitObjects& objects)
	{
		bool current_from_right = true;
		HitObjects result;
		for (const auto& osu_object : objects.data)
		{
			for (const auto obj = convert_hit_object(osu_object, current_from_right); 
				const auto& object : obj)
			{
				result.data.emplace_hint(
					Utilities::Container::get_last_element_iterator(result.data),
					object.time, object);
			}
		}
		return result;
	}

	void convert_mapset(std::filesystem::path path, const std::filesystem::path& output)
	{
		path = Utilities::Path::normalize_path(path);

		const Beatmap beatmap(path.string());
		std::ofstream writer(output);
		if (!writer)
			THROW_ERROR(Logging::Exceptions::File::File_Open_Failed(output));

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
