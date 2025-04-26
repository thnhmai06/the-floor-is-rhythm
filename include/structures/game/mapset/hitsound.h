#pragma once
#include <osu!parser/Parser/Structures/Beatmap/Objects/HitObject.hpp>
#include "structures/type.hpp"

// lưu trữ HitSound và Hitsample giống như Osu!
// p/s: code trong Osu!parser/Convert/Structures/Playing/HitObjects.hpp
// phần HitSound và HitSample là do mình viết :D

namespace Structures::Game::Beatmap::Hitsound
{
	using namespace Types::Game::HitSound;

	struct HitSound
	{
		bool normal = false;
		bool whistle = false;
		bool finish = false;
		bool clap = false;

		void read(const std::int32_t& hit_sound_int);
		[[nodiscard]] bool is_hit_sound_type_played(const HitSoundType& hit_sound_type) const;
		[[nodiscard]] std::int32_t to_int() const;

		HitSound() = default;
		explicit HitSound(const std::int32_t& hit_sound_int);
		HitSound& operator= (const OsuParser::Beatmap::Objects::HitObject::Hitsound& hs);
		size_t operator()(const HitSound& hs) const;
	};

	struct TimingSample
	{
		HitSampleType set = HitSampleType::Normal;
		int32_t index = 0;
		uint8_t volume = 100;
	};

	struct HitSample
	{
		HitSampleType normal_set = HitSampleType::NoCustom;
		HitSampleType addition_set = HitSampleType::NoCustom;
		int32_t index = 0;
		uint8_t volume = 100;
		std::string file_name = {};

		[[nodiscard]] std::string to_string() const;
		// Có thể bạn đang nhầm với get_hit_sound_filename()?
		[[nodiscard]] std::pair<SampleSetType, HitSampleType>
			get_used_sample_set(
				const HitSoundType& hit_sound_type,
				const HitSampleType& timing_sample_type,
				const HitSampleType& mapset_sample_type) const;
		[[nodiscard]] std::vector<std::pair<SampleSetType, HitSampleType>>
			get_used_sample_set(
				const HitSound& hit_sound,
				const HitSampleType& timing_sample_type,
				const HitSampleType& mapset_sample_type) const;

		explicit HitSample(
			const HitSampleType& normal_set = HitSampleType::NoCustom, 
			const HitSampleType& addition_set = HitSampleType::NoCustom, 
			const int32_t& index = 0, const uint8_t& volume = MIX_MAX_VOLUME, 
			std::string file_name = {});
		explicit HitSample(const std::string& hitsample_str);
		//size_t operator()(const HitSample& hs) const;

		HitSample& operator= (const OsuParser::Beatmap::Objects::HitObject::HitSample& hs);
		HitSample& operator= (const OsuParser::Beatmap::Objects::HitObject::HitObject::SliderParams::EdgeHitsound::SampleSet& hs);
	};

	 std::string get_hit_sound_filename(const HitSoundType& hit_sound_type, const HitSample& hit_sample,
		const TimingSample& timing_sample, const HitSampleType& mapset_sample);
	 std::vector<std::string> get_hit_sound_filename(const HitSound& hit_sound, const HitSample& hit_sample, 
		 const TimingSample& timing_sample, const HitSampleType& mapset_sample);
}