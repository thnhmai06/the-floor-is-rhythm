#include "work/convert/osu/mapset.h" // Header
#include <osu!parser/Parser/Beatmap.hpp>
#include "format/file.h"
#include "structures/game/mapset/metadata.h"
#include "structures/game/mapset/hitobject.h"
#include "structures/game/mapset/timing.h"
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "structures/type.hpp"
#include "utilities.hpp"

namespace Work::Convert::osu
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
		case Objects::TimingPoint::HitSampleType::NO_CUSTOM:
			result.sample_set = Structures::Types::Game::HitSound::HitSampleType::NoCustom;
			break;
		case Objects::TimingPoint::HitSampleType::NORMAL:
			result.sample_set = Structures::Types::Game::HitSound::HitSampleType::Normal;
			break;
		case Objects::TimingPoint::HitSampleType::SOFT:
			result.sample_set = Structures::Types::Game::HitSound::HitSampleType::Soft;
			break;
		case Objects::TimingPoint::HitSampleType::DRUM:
			result.sample_set = Structures::Types::Game::HitSound::HitSampleType::Drum;
			break;
		}

		return result;
	}
	static Difficulty convert_difficulty(const Sections::Difficulty::DifficultySection& difficulty)
	{
		Difficulty result;
		if (!difficulty.OverallDifficulty.empty()) result.od = std::stof(difficulty.OverallDifficulty);
		if (!difficulty.HPDrainRate.empty()) result.hp = std::stof(difficulty.HPDrainRate);
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
		result.sample.set = static_cast<HitSampleType>(timing_point.SampleSet);
		result.sample.index = timing_point.SampleIndex;
		result.sample.volume = std::round(Utilities::Math::to_value(static_cast<float>(timing_point.Volume) / 100.0f, 0, MIX_MAX_VOLUME));
		result.kiai = timing_point.Effects.kiai;
		return result;
	}
	static TimingPoints convert_timing_points(const std::vector<Objects::TimingPoint::TimingPoint>& timing_points)
	{
		TimingPoints result;
		for (const auto& timing_point : timing_points)
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
		if (!object.Type.Slider && !object.Type.HitCircle) return res;

		if (object.Type.HitCircle)
		{
			Floor floor;
			floor.time = object.Time;
			if (object.Type.IsNewCombo)
				current_from_right = !current_from_right;
			floor.is_kat = current_from_right;
			floor.hit_sound = object.Hitsound;
			floor.hit_sample = object.Hitsample;
			res.push_back(std::move(floor));

			return res;
		}
		if (object.Type.Slider)
		{
			const double one_slide_time = (object.EndTime.value() - object.Time) / object.SliderParameters.value().Slides;
			double current_time = object.Time;
			for (int32_t slide = 1; slide <= object.SliderParameters.value().Slides; ++slide, current_time += one_slide_time)
			{
				Floor floor;
				floor.time = std::round(current_time);
				if ((slide == 1 && object.Type.IsNewCombo) || slide > 1)
					current_from_right = !current_from_right;
				floor.is_kat = current_from_right;
				if (slide == 1)
				{
					floor.hit_sound = object.Hitsound;
					floor.hit_sample = object.Hitsample;
				} else
				{
					floor.hit_sound = object.SliderParameters.value().edgeHitsounds[slide - 1].Sound;
					floor.hit_sample = object.SliderParameters.value().edgeHitsounds[slide - 1].Sample;
				}
				res.push_back(std::move(floor));
			}
		}
		return res;
	}
	static HitObjects convert_hit_objects(const std::vector<Objects::HitObject::HitObject>& objects)
	{
		bool current_from_right = true;
		HitObjects result;
		for (const auto& osu_object : objects)
		{
			const auto objects = convert_hit_object(osu_object, current_from_right);
			for (const auto& object : objects)
			{
				result.data.emplace_hint(
					Utilities::Container::get_last_element_iterator(result.data),
					object.time, object);
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
