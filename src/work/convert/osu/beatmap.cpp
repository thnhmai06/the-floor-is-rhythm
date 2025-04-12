#include "work/convert/osu/beatmap.h" // Header
#include <osu!parser/Parser/Beatmap.hpp>
#include <fstream>
#include "format/file.h"
#include "structures/game/beatmap/metadata.h"
#include "structures/game/beatmap/hitobject.h"
#include "structures/game/beatmap/timing.h"
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "structures/types.h"
#include "utilities.h"

namespace Work::Convert::osu
{
	using namespace Structures::Game::Beatmap::Metadata;
	static Structures::Types::Game::Direction::Rotation get_rotation(const uint8_t colour_hax)
	{
		// Loại bỏ hai hướng là cùng phương là hướng hiện tại và hướng ngược với nó
		// Ta còn lại hai hướng phương vuông góc, colour_hax sẽ quyết định là hướng nào (vì còn 2 hướng nên quyết định bằng tính chẵn lẻ)
		// colour_hax: 1 -> 7 (3 cùng tính chẵn lẻ, 4 không cùng) nên xác suất là như nhau khi đầy đủ (trừ khi bên beatmap osu lẻ combo color)
		if (colour_hax == 0) return Structures::Types::Game::Direction::Rotation::NO_ROTATE;
		return Utilities::Math::is_bit_enabled(colour_hax, 1)
			? Structures::Types::Game::Direction::Rotation::ROTATE_90
			: Structures::Types::Game::Direction::Rotation::ROTATE_270;
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
		result.source.append(Format::FileFormat::Beatmap::ConvertInformation::osu::SOURCE);
		result.tags = metadata.Tags;
		return result;
	}
	//! HitObjects
	using namespace Structures::Game::Beatmap::HitObjects;
	static Slider convert_hit_object_slider(const Parser::HitObject& object)
	{
		Slider slider;
		slider.time = object.Time;
		slider.end_time = slider.time;
		if (object.EndTime.has_value())
			slider.end_time = object.EndTime.value();
		slider.rotation = get_rotation(static_cast<uint8_t>(object.Type.ColourHax));
		slider.combo_jump = static_cast<uint8_t>(object.Type.ColourHax);

		// Bên dưới là thêm slider curve dựa trên số lần lặp.
		// Nah slides trong osu! nhiều khi nó lặp nhanh quá, nên mình không muốn thêm curve
		// dựa trên nó. So... bỏ slider curve nhé :D
		/*
		if (object.SliderParameters.has_value())
		{
			const float add_time_unit =
				static_cast<float>(slider.end_time - slider.time) / object.SliderParameters.value().Slides;
			for (auto i = 1; i < object.SliderParameters.value().Slides - 1; ++i)
			{
				Slider::SliderCurve curve;
				curve.after = static_cast<int32_t>(add_time_unit * static_cast<float>(i));
				curve.rotation = (current_curve_rotation ?
					Structures::Types::Game::Direction::Rotation::ROTATE_270 :
					Structures::Types::Game::Direction::Rotation::ROTATE_90);
				current_curve_rotation = !current_curve_rotation;
				slider.curves.push_back(curve);
			}
		}
		*/

		slider.hit_sound = object.Hitsound;
		slider.hit_sample = object.Hitsample;
		return slider;
	}
	static Floor convert_hit_object_floor(const Parser::HitObject& object)
	{
		Floor floor;
		floor.end_time = floor.time = object.Time;
		floor.rotation = get_rotation(static_cast<uint8_t>(object.Type.ColourHax));
		floor.combo_jump = static_cast<uint8_t>(object.Type.ColourHax);
		floor.hit_sound = object.Hitsound;
		floor.hit_sample = object.Hitsample;
		return floor;
	}
	static HitObjects convert_hit_objects(const std::vector<Parser::HitObject>& objects)
	{
		HitObjects result;
		for (const Parser::HitObject& osu_object : objects)
		{
			const auto back_itr = (result.empty()) ? (result.end()) : (std::prev(result.end()));
			if (osu_object.Type.HitCircle)
				result.emplace_hint(back_itr, osu_object.Time, convert_hit_object_floor(osu_object));
			else result.emplace_hint(back_itr, osu_object.Time, convert_hit_object_slider(osu_object)); // slider, spinner...
		}
		return result;
	}
	//! TimingPoints
	using namespace Structures::Game::Beatmap::TimingPoints;
	using Structures::Game::Beatmap::Hitsound::SampleSetType;
	static TimingPoint convert_timing_point(const Parser::TimingPoint& timing_point)
	{
		TimingPoint result;
		result.time = timing_point.Time;
		result.beat_length = static_cast<float>(timing_point.BeatLength);
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
			result.emplace_hint(back_itr, timing_point.Time, convert_timing_point(timing_point));
		}
		return result;
	}

	void convert_beatmap(const char* file_name, const char* output)
	{
		Parser::Beatmap beatmap(file_name);
		std::ofstream writer(output);
		if (!writer)
			THROW_ERROR(Logging::Exceptions::FileExceptions::File_Open_Failed(output));
		// Version
		writer << Format::FileFormat::Beatmap::FORMAT_VERSION << '\n';
		writer << Format::FileFormat::Beatmap::ConvertInformation::osu::VERSION << beatmap.Version << '\n';
		writer << '\n';
		// Contents
		convert_general(beatmap.General).write(writer);
		convert_difficulty(beatmap.Difficulty).write(writer);
		convert_metadata(beatmap.Metadata).write(writer);
		convert_timing_points(beatmap.TimingPoints).write(writer);
		convert_hit_objects(beatmap.HitObjects).write(writer);

		writer.close();
	}
}