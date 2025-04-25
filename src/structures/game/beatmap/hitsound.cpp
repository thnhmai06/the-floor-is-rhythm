#include "structures/game/beatmap/hitsound.h" // Header
#include <bitset>
#include "format/file.h"
#include "utilities.hpp"

namespace Structures::Game::Beatmap::Hitsound
{
	//! HitSound
	void HitSound::read(const std::int32_t& hit_sound_int)
	{
		const auto bitmap = std::bitset<4>(hit_sound_int);

		normal = bitmap[static_cast<uint8_t>(HitSoundType::NORMAL)];
		whistle = bitmap[static_cast<uint8_t>(HitSoundType::WHISTLE)];
		finish = bitmap[static_cast<uint8_t>(HitSoundType::FINISH)];
		clap = bitmap[static_cast<uint8_t>(HitSoundType::CLAP)];
	}
	bool HitSound::is_hit_sound_type_played(const HitSoundType& hit_sound_type) const
	{
		switch (hit_sound_type)
		{
		case HitSoundType::NORMAL:
			return normal;
		case HitSoundType::WHISTLE:
			return whistle;
		case HitSoundType::FINISH:
			return finish;
		case HitSoundType::CLAP:
			return clap;
		}
		return false;
	}
	std::int32_t HitSound::to_int() const
	{
		auto bitmap = std::bitset<4>(0);

		bitmap[static_cast<uint8_t>(HitSoundType::NORMAL)] = normal;
		bitmap[static_cast<uint8_t>(HitSoundType::WHISTLE)] = whistle;
		bitmap[static_cast<uint8_t>(HitSoundType::FINISH)] = finish;
		bitmap[static_cast<uint8_t>(HitSoundType::CLAP)] = clap;

		return bitmap.to_ulong();
	}
	size_t HitSound::operator()(const HitSound& hs) const
	{
		return std::hash<int32_t>()(hs.to_int());
	}

	HitSound::HitSound(const std::int32_t& hit_sound_int)
	{
		read(hit_sound_int);
	}

	HitSound& HitSound::operator= (const OsuParser::Beatmap::Objects::HitObject::Hitsound& hs)
	{
		whistle = hs.Whistle;
		finish = hs.Finish;
		clap = hs.Clap;
		return *this;
	}

	//! Hitsample
	HitSample& HitSample::operator= (const OsuParser::Beatmap::Objects::HitObject::HitSample& hs)
	{
		normal_set = static_cast<HitSampleType>(static_cast<int32_t>(hs.NormalSet));
		addition_set = static_cast<HitSampleType>(static_cast<int32_t>(hs.AdditionSet));
		index = hs.Index;
		volume = hs.Volume;
		file_name = hs.Filename;
		return *this;
	}
	std::pair<HitSample::SampleSetType, std::pair<HitSample::SampleSetType, HitSampleType>>
		HitSample::get_used_sample_set(const HitSoundType& hit_sound_type, const HitSampleType& current_timing_point_sample_type) const
	{
		const auto sample_set_type = (hit_sound_type == HitSoundType::NORMAL) ? SampleSetType::NORMAL : SampleSetType::ADDITION;
		const auto used_sample_set =
			(sample_set_type == SampleSetType::ADDITION && addition_set != HitSampleType::NO_CUSTOM)
			? SampleSetType::ADDITION
			: (normal_set != HitSampleType::NO_CUSTOM // ở đay bản chất là NORMAL
				? SampleSetType::NORMAL
				: SampleSetType::TIMING_POINTS);
		HitSampleType used_hit_sample;
		switch (used_sample_set)
		{
		case SampleSetType::NORMAL:
			used_hit_sample = normal_set;
			break;
		case SampleSetType::ADDITION:
			used_hit_sample = addition_set;
			break;
		case SampleSetType::TIMING_POINTS:
			used_hit_sample = current_timing_point_sample_type;
			break;
		}
		return { sample_set_type, {used_sample_set, used_hit_sample} };
	}
	std::string HitSample::to_string() const
	{
		std::string result;
		result.append(std::to_string(static_cast<int32_t>(normal_set)));
		result.push_back(Format::File::Floor::Mapset::HitObjects::HitSample::DELIMITER);
		result.append(std::to_string(static_cast<int32_t>(addition_set)));
		result.push_back(Format::File::Floor::Mapset::HitObjects::HitSample::DELIMITER);
		result.append(std::to_string(index));
		result.push_back(Format::File::Floor::Mapset::HitObjects::HitSample::DELIMITER);
		result.append(std::to_string(volume));
		result.push_back(Format::File::Floor::Mapset::HitObjects::HitSample::DELIMITER);
		if (!file_name.empty()) result.append(file_name);
		return result;
	}
	HitSample::HitSample(const std::string& hitsample_str)
	{
		if (hitsample_str.empty())
			return; // không được viết -> mặc định
		const auto list = Utilities::String::split(hitsample_str, Format::File::Floor::Mapset::HitObjects::HitSample::DELIMITER);
		normal_set = static_cast<HitSampleType>(std::stoi(list[0]));
		addition_set = static_cast<HitSampleType>(std::stoi(list[1]));
		index = std::stoi(list[2]);
		volume = std::stoi(list[3]);
		if (list.size() > 4) file_name = list[4];
	}

	size_t HitSample::operator()(const HitSample& hs) const
	{
		const auto normal_hash = std::hash<uint8_t>()(static_cast<uint8_t>(normal_set));
		const auto addition_hash = std::hash<uint8_t>()(static_cast<uint8_t>(addition_set));
		const auto index_hash = std::hash<int>()(index);
		const auto filename_hash = std::hash<std::string>()(file_name);

		return normal_hash ^ addition_hash ^ index_hash ^ filename_hash;
	}


	//! global
	std::string to_string(const HitSoundType& hit_sound_type)
	{
		std::string result;
		switch (hit_sound_type)
		{
		case HitSoundType::NORMAL:
			result = "normal";
			break;
		case HitSoundType::WHISTLE:
			result = "whistle";
			break;
		case HitSoundType::FINISH:
			result = "finish";
			break;
		case HitSoundType::CLAP:
			result = "clap";
			break;
		}
		return result;
	}
	std::string to_string(const HitSampleType& hit_sample_type)
	{
		std::string result;
		switch (hit_sample_type)
		{
		case HitSampleType::NO_CUSTOM:
			result = "";
			break;
		case HitSampleType::NORMAL:
			result = "normal";
			break;
		case HitSampleType::SOFT:
			result = "soft";
			break;
		case HitSampleType::DRUM:
			result = "drum";
			break;
		}
		return result;
	}
	static std::string get_hit_sound_filename(const HitSoundType& hit_sound_type,
		const HitSampleType& hit_sample_type, const int32_t& index = 1) // chỉ dùng khi biết HitSampleType cụ thể cần dùng là gì
	{
		std::string result;
		result.append(to_string(hit_sample_type));
		result.append("-hit");
		result.append(to_string(hit_sound_type));
		if (index != 0 && index != 1)
			result.append(std::to_string(index));
		return result;
	}
	std::string get_hit_sound_filename(const HitSoundType& hit_sound_type, const HitSample& hit_sample,
		const HitSampleType& current_timing_point_sample_type)
	{
		if (!hit_sample.file_name.empty())
		{
			// file_name chỉ được sử dụng cho hit_sound_type NORMAL
			if (hit_sound_type == HitSoundType::NORMAL)
				return hit_sample.file_name;
			return {};
		}

		const auto [used_sample_set, used_hit_sample] = hit_sample.get_used_sample_set(
			hit_sound_type, current_timing_point_sample_type).second;
		return get_hit_sound_filename(hit_sound_type, used_hit_sample, hit_sample.index);
	}
	std::vector<std::string> get_hitsound_filename(const HitSound& hit_sound, const HitSample& hit_sample, const HitSampleType& current_timing_point_sample_type)
	{
		std::vector<std::string> result;
		if (hit_sound.normal)
			result.push_back(get_hit_sound_filename(HitSoundType::NORMAL, hit_sample, current_timing_point_sample_type));
		if (hit_sound.whistle)
			result.push_back(get_hit_sound_filename(HitSoundType::WHISTLE, hit_sample, current_timing_point_sample_type));
		if (hit_sound.finish)
			result.push_back(get_hit_sound_filename(HitSoundType::FINISH, hit_sample, current_timing_point_sample_type));
		if (hit_sound.clap)
			result.push_back(get_hit_sound_filename(HitSoundType::CLAP, hit_sample, current_timing_point_sample_type));
		return result;
	}
}