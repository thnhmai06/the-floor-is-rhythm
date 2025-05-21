#include "structures/game/mapset/hitsound.h" // Header
#include <bitset>
#include "format/file.h"
#include "utilities.h"

namespace Structures::Game::Beatmap::Hitsound
{
	//! Additions
	void Additions::read(const std::int32_t& hit_sound_int)
	{
		const auto bitmap = std::bitset<4>(hit_sound_int);

		normal = bitmap[static_cast<uint8_t>(Addition::Normal)];
		whistle = bitmap[static_cast<uint8_t>(Addition::Whistle)];
		finish = bitmap[static_cast<uint8_t>(Addition::Finish)];
		clap = bitmap[static_cast<uint8_t>(Addition::Clap)];

		if (!normal && !whistle && !finish && !clap) normal = true;
	}
	bool Additions::has_addition(const Addition& additions) const
	{
		switch (additions)
		{
		case Addition::Normal:
			return normal;
		case Addition::Whistle:
			return whistle;
		case Addition::Finish:
			return finish;
		case Addition::Clap:
			return clap;
		}
		return false;
	}
	std::int32_t Additions::to_int() const
	{
		auto bitmap = std::bitset<4>(0);

		bitmap[static_cast<uint8_t>(Addition::Normal)] = normal;
		bitmap[static_cast<uint8_t>(Addition::Whistle)] = whistle;
		bitmap[static_cast<uint8_t>(Addition::Finish)] = finish;
		bitmap[static_cast<uint8_t>(Addition::Clap)] = clap;

		return static_cast<int32_t>(bitmap.to_ulong());
	}
	size_t Additions::operator()(const Additions& hs) const
	{
		return std::hash<int32_t>()(hs.to_int());
	}

	Additions::Additions(const std::int32_t& hit_sound_int)
	{
		read(hit_sound_int);
	}

	Additions& Additions::operator= (const OsuParser::Beatmap::Objects::HitObject::Additions& hs)
	{
		whistle = hs.Whistle;
		finish = hs.Finish;
		clap = hs.Clap;
		return *this;
	}

	//! HitSample
	std::pair<SampleSetType, SampleSet>
		HitSample::get_used_sample_set(
			const Addition& addition,
			const SampleSet& timing_sample_type,
			const SampleSet& mapset_sample_type) const
	{
		const auto used_sample_set = (addition == Addition::Normal) ? SampleSetType::Normal : SampleSetType::Addition;
		SampleSet used_hit_sample;

		if (used_sample_set == SampleSetType::Normal)
		{
			// Dùng của Timing Point
			if (normal_set == SampleSet::NoCustom)
			{
				// Dùng của Beatmap
				if (timing_sample_type == SampleSet::NoCustom)
				{
					used_hit_sample = mapset_sample_type;
				}
				else used_hit_sample = timing_sample_type;
			}
			else used_hit_sample = normal_set;
		}
		else
		{
			// Dùng của Timing Point
			if (addition_set == SampleSet::NoCustom)
			{
				// Dùng của Beatmap
				if (timing_sample_type == SampleSet::NoCustom)
				{
					used_hit_sample = mapset_sample_type;
				}
				else used_hit_sample = timing_sample_type;
			}
			else used_hit_sample = addition_set;
		}

		return { used_sample_set, used_hit_sample };
	}
	std::vector<std::pair<SampleSetType, SampleSet>>
		HitSample::get_used_sample_set(
			const Additions& additions,
			const SampleSet& timing_sample_type,
			const SampleSet& mapset_sample_type) const
	{
		std::vector<std::pair<SampleSetType, SampleSet>> res{};
		res.push_back(get_used_sample_set(Addition::Normal, timing_sample_type, mapset_sample_type)); //! âm thanh normal luôn được phát
		// additions
		if (additions.whistle)
			res.push_back(get_used_sample_set(Addition::Whistle, timing_sample_type, mapset_sample_type));
		if (additions.finish)
			res.push_back(get_used_sample_set(Addition::Finish, timing_sample_type, mapset_sample_type));
		if (additions.clap)
			res.push_back(get_used_sample_set(Addition::Clap, timing_sample_type, mapset_sample_type));
		return res;
	}
	void HitSample::read(const std::string& hitsample_str)
	{
		if (hitsample_str.empty()) return; // không được viết -> theo mặc định
		const auto list = Utilities::String::split(hitsample_str, Format::File::Floor::Mapset::HitObjects::HitSample::AND);
		normal_set = static_cast<SampleSet>(std::stoi(list[0]));
		addition_set = static_cast<SampleSet>(std::stoi(list[1]));
		index = std::stoi(list[2]);
		volume.percent = std::stod(list[3]);
		if (list.size() > 4) file_name = list[4];
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
		result.append(std::to_string(volume.percent));
		result.push_back(Format::File::Floor::Mapset::HitObjects::HitSample::AND);
		if (!file_name.empty()) result.append(file_name);
		return result;
	}
	HitSample& HitSample::operator=(const OsuParser::Beatmap::Objects::HitObject::SliderSample& slide_hs)
	{
		normal_set = static_cast<SampleSet>(slide_hs.NormalSet);
		addition_set = static_cast<SampleSet>(slide_hs.AdditionSet);
		return *this;
	}
	HitSample& HitSample::operator=(const OsuParser::Beatmap::Objects::HitObject::HitSample& hs)
	{
		normal_set = static_cast<SampleSet>(hs.NormalSet);
		addition_set = static_cast<SampleSet>(hs.AdditionSet);
		index = hs.Index;
		volume.percent = static_cast<double>(hs.Volume);
		file_name = hs.Filename;
		return *this;
	}
	HitSample::HitSample(
		const SampleSet& normal_set, const SampleSet& addition_set,
		const int32_t& index, const double& volume, std::string file_name)
		: normal_set(normal_set), addition_set(addition_set), index(index), file_name(std::move(file_name))
	{
		this->volume.percent = volume;
	}
	HitSample::HitSample(const std::string& hitsample_str) { read(hitsample_str); }

	//! global
	static std::string get_hit_sound_filename(const Addition& additions,
		const SampleSet& sample_set, const int32_t& index) // chỉ dùng khi biết SampleSet cụ thể cần dùng là gì
	{
		std::string result;
		result.append(to_string(sample_set));
		result.append("-hit");
		result.append(to_string(additions));
		if (index != 0 && index != 1)
			result.append(std::to_string(index));
		return result;
	}
	std::string get_hit_sound_filename(
		const Addition& addition, const HitSample& hit_sample,
		const TimingSample& timing_sample, const SampleSet& mapset_sample,
		const Engine::Audio::Memory::EffectMemory& beatmap_effect)
	{
		if (!hit_sample.file_name.empty())
		{
			// file_name chỉ được sử dụng cho addition Normal
			if (addition == Addition::Normal)
				return hit_sample.file_name;
			return {};
		}

		const auto [used_sample_set, used_hit_sample] =
			hit_sample.get_used_sample_set(
				addition, timing_sample.set, mapset_sample);

		std::string res{};
		if (hit_sample.index == 0)
		{
			res = get_hit_sound_filename(addition, used_hit_sample, timing_sample.index);
			if (beatmap_effect.data.contains(res))
				return res;
		}
		res = get_hit_sound_filename(addition, used_hit_sample, hit_sample.index);
		return res;
	}
	std::vector<std::string> get_hit_sound_filename(
		const Additions& additions, const HitSample& hit_sample,
		const TimingSample& timing_sample, const SampleSet& mapset_sample,
		const Engine::Audio::Memory::EffectMemory& beatmap_effect)
	{
		std::vector<std::string> result;
		result.push_back(get_hit_sound_filename(Addition::Normal, hit_sample, timing_sample, mapset_sample, beatmap_effect)); //! âm thanh normal luôn được phát
		if (additions.whistle)
			result.push_back(get_hit_sound_filename(Addition::Whistle, hit_sample, timing_sample, mapset_sample, beatmap_effect));
		if (additions.finish)
			result.push_back(get_hit_sound_filename(Addition::Finish, hit_sample, timing_sample, mapset_sample, beatmap_effect));
		if (additions.clap)
			result.push_back(get_hit_sound_filename(Addition::Clap, hit_sample, timing_sample, mapset_sample, beatmap_effect));
		return result;
	}
	double get_hit_sample_volume(const HitSample& hit_sample, const TimingSample& timing_sample)
	{
		if (Utilities::Math::is_equal(hit_sample.volume.percent, 0))
			return timing_sample.volume.percent;
		return hit_sample.volume.percent;
	}
}
