#include "structures/game/mapset/hitsound.h" // Header
#include <bitset>
#include "format/file.h"
#include "utilities.hpp"

namespace Structures::Game::Beatmap::Hitsound
{
	//! HitSound
	void HitSound::read(const std::int32_t& hit_sound_int)
	{
		const auto bitmap = std::bitset<4>(hit_sound_int);

		normal = bitmap[static_cast<uint8_t>(HitSoundType::Normal)];
		whistle = bitmap[static_cast<uint8_t>(HitSoundType::Whistle)];
		finish = bitmap[static_cast<uint8_t>(HitSoundType::Finish)];
		clap = bitmap[static_cast<uint8_t>(HitSoundType::Clap)];
	}
	bool HitSound::is_hit_sound_type_played(const HitSoundType& hit_sound_type) const
	{
		switch (hit_sound_type)
		{
		case HitSoundType::Normal:
			return normal;
		case HitSoundType::Whistle:
			return whistle;
		case HitSoundType::Finish:
			return finish;
		case HitSoundType::Clap:
			return clap;
		}
		return false;
	}
	std::int32_t HitSound::to_int() const
	{
		auto bitmap = std::bitset<4>(0);

		bitmap[static_cast<uint8_t>(HitSoundType::Normal)] = normal;
		bitmap[static_cast<uint8_t>(HitSoundType::Whistle)] = whistle;
		bitmap[static_cast<uint8_t>(HitSoundType::Finish)] = finish;
		bitmap[static_cast<uint8_t>(HitSoundType::Clap)] = clap;

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

	//! HitSample
	HitSample& HitSample::operator= (const OsuParser::Beatmap::Objects::HitObject::HitSample& hs)
	{
		normal_set = static_cast<HitSampleType>(static_cast<int32_t>(hs.NormalSet));
		addition_set = static_cast<HitSampleType>(static_cast<int32_t>(hs.AdditionSet));
		index = hs.Index;
		volume =  std::round(Utilities::Math::to_value(static_cast<float>(hs.Volume) / 100.0f, 0, MIX_MAX_VOLUME));
		file_name = hs.Filename;
		return *this;
	}
	std::pair<SampleSetType, HitSampleType>
		HitSample::get_used_sample_set(
			const HitSoundType& hit_sound_type,
			const HitSampleType& timing_sample_type,
			const HitSampleType& mapset_sample_type) const
	{
		const auto used_sample_set = (hit_sound_type == HitSoundType::Normal) ? SampleSetType::Normal : SampleSetType::Addition;
		HitSampleType used_hit_sample;

		if (used_sample_set == SampleSetType::Normal)
		{
			// Dùng của Timing Point
			if (normal_set == HitSampleType::NoCustom)
			{
				// Dùng của Beatmap
				if (timing_sample_type == HitSampleType::NoCustom)
				{
					used_hit_sample = mapset_sample_type;
				}
				else used_hit_sample = timing_sample_type;
			}
			else used_hit_sample = normal_set;
		} else
		{
			// Dùng của Timing Point
			if (addition_set == HitSampleType::NoCustom)
			{
				// Dùng của Beatmap
				if (timing_sample_type == HitSampleType::NoCustom)
				{
					used_hit_sample = mapset_sample_type;
				}
				else used_hit_sample = timing_sample_type;
			}
			else used_hit_sample = addition_set;
		}

		return { used_sample_set, used_hit_sample };
	}
	std::vector<std::pair<SampleSetType, HitSampleType>>
	HitSample::get_used_sample_set(
		const HitSound& hit_sound, 
		const HitSampleType& timing_sample_type, 
		const HitSampleType& mapset_sample_type) const
	{
		std::vector<std::pair<SampleSetType, HitSampleType>> res{};
		if (hit_sound.normal)
			res.push_back(get_used_sample_set(HitSoundType::Normal, timing_sample_type, mapset_sample_type));
		if (hit_sound.whistle)
			res.push_back(get_used_sample_set(HitSoundType::Whistle, timing_sample_type, mapset_sample_type));
		if (hit_sound.finish)
			res.push_back(get_used_sample_set(HitSoundType::Finish, timing_sample_type, mapset_sample_type));
		if (hit_sound.clap)
			res.push_back(get_used_sample_set(HitSoundType::Clap, timing_sample_type, mapset_sample_type));
		return res;
	}
	std::string HitSample::to_string() const
	{
		std::string result;
		result.append(std::to_string(static_cast<int32_t>(normal_set)));
		result.push_back(Format::File::Floor::Mapset::HitObjects::HitSample::AND);
		result.append(std::to_string(static_cast<int32_t>(addition_set)));
		result.push_back(Format::File::Floor::Mapset::HitObjects::HitSample::AND);
		result.append(std::to_string(index));
		result.push_back(Format::File::Floor::Mapset::HitObjects::HitSample::AND);
		result.append(std::to_string(static_cast<uint32_t>(volume)));
		result.push_back(Format::File::Floor::Mapset::HitObjects::HitSample::AND);
		if (!file_name.empty()) result.append(file_name);
		return result;
	}
	HitSample::HitSample(
		const HitSampleType& normal_set, const HitSampleType& addition_set, 
		const int32_t& index, const uint8_t& volume, std::string file_name)
		: normal_set(normal_set), addition_set(addition_set), index(index), volume(volume), file_name(std::move(file_name))
	{
	}
	HitSample::HitSample(const std::string& hitsample_str)
	{
		if (hitsample_str.empty()) return; // không được viết -> theo mặc định
		const auto list = Utilities::String::split(hitsample_str, Format::File::Floor::Mapset::HitObjects::HitSample::AND);
		normal_set = static_cast<HitSampleType>(std::stoi(list[0]));
		addition_set = static_cast<HitSampleType>(std::stoi(list[1]));
		index = std::stoi(list[2]);
		volume = std::stoi(list[3]);
		if (list.size() > 4) file_name = list[4];
	}

	HitSample::HitSample(
		const OsuParser::Beatmap::Objects::HitObject::HitObject::SliderParams::EdgeHitsound::SampleSet& slide_hs, 
		const OsuParser::Beatmap::Objects::HitObject::HitSample& slider_hs)
	{
		using OsuParser::Beatmap::Objects::TimingPoint::HitSampleType;

		if (slide_hs.NormalSet == HitSampleType::NO_CUSTOM && slide_hs.AdditionSet == HitSampleType::NO_CUSTOM) 
			*this = slider_hs;
		normal_set = to_floor_hit_sample(slide_hs.NormalSet);
		addition_set = to_floor_hit_sample(slide_hs.AdditionSet);
	}

	/*size_t HitSample::operator()(const HitSample& hs) const
	{
		const auto normal_hash = std::hash<uint8_t>()(static_cast<uint8_t>(normal_set));
		const auto addition_hash = std::hash<uint8_t>()(static_cast<uint8_t>(addition_set));
		const auto index_hash = std::hash<int>()(index);
		const auto filename_hash = std::hash<std::string>()(file_name);

		return normal_hash ^ addition_hash ^ index_hash ^ filename_hash;
	}*/

	//! global
	static std::string get_hit_sound_filename(const HitSoundType& hit_sound_type,
		const HitSampleType& hit_sample_type, const int32_t& index) // chỉ dùng khi biết HitSampleType cụ thể cần dùng là gì
	{
		std::string result;
		result.append(to_string(hit_sample_type));
		result.append("-hit");
		result.append(to_string(hit_sound_type));
		if (index != 0 && index != 1)
			result.append(std::to_string(index));
		return result;
	}
	std::string get_hit_sound_filename(
		const HitSoundType& hit_sound_type, const HitSample& hit_sample,
		const TimingSample& timing_sample, const HitSampleType& mapset_sample,
		const Audio::EffectMemory& beatmap_effect)
	{
		if (!hit_sample.file_name.empty())
		{
			// file_name chỉ được sử dụng cho hit_sound_type Normal
			if (hit_sound_type == HitSoundType::Normal)
				return hit_sample.file_name;
			return {};
		}

		const auto [used_sample_set, used_hit_sample] = 
			hit_sample.get_used_sample_set(
				hit_sound_type, timing_sample.set, mapset_sample);

		std::string res{};
		if (hit_sample.index == 0)
		{
			res = get_hit_sound_filename(hit_sound_type, used_hit_sample, timing_sample.index);
			if (beatmap_effect.data.contains(res))
				return res;
		}
		res = get_hit_sound_filename(hit_sound_type, used_hit_sample, hit_sample.index);
		return res;
	}
	std::vector<std::string> get_hit_sound_filename(
		const HitSound& hit_sound, const HitSample& hit_sample, 
		const TimingSample& timing_sample, const HitSampleType& mapset_sample,
		const Audio::EffectMemory& beatmap_effect)
	{
		std::vector<std::string> result;
		if (hit_sound.normal)
			result.push_back(get_hit_sound_filename(HitSoundType::Normal, hit_sample, timing_sample, mapset_sample, beatmap_effect));
		if (hit_sound.whistle)
			result.push_back(get_hit_sound_filename(HitSoundType::Whistle, hit_sample, timing_sample, mapset_sample, beatmap_effect));
		if (hit_sound.finish)
			result.push_back(get_hit_sound_filename(HitSoundType::Finish, hit_sample, timing_sample, mapset_sample, beatmap_effect));
		if (hit_sound.clap)
			result.push_back(get_hit_sound_filename(HitSoundType::Clap, hit_sample, timing_sample, mapset_sample, beatmap_effect));
		return result;
	}
	uint8_t get_hit_sample_volume(const HitSample& hit_sample, const TimingSample& timing_sample)
	{
		if (hit_sample.volume == 0)
			return timing_sample.volume;
		return hit_sample.volume;
	}
}
