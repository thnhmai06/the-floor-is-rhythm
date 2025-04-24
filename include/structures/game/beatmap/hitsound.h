#pragma once
#include <osu!parser/Parser/Structures/Beatmap/Objects/HitObject.hpp>

// lưu trữ HitSound và Hitsample giống như osu!
// p/s: code trong osu!parser/Convert/Structures/Playing/HitObjects.hpp
// phần HitSound và HitSample là do mình viết :D

namespace Structures::Game::Beatmap::Hitsound
{
	enum class HitSoundType : std::uint8_t
	{
		NORMAL = 1 << 0,
		WHISTLE = 1 << 1,
		FINISH = 1 << 2,
		CLAP = 1 << 3
	};
	enum class HitSampleType : std::uint8_t
	{
		NO_CUSTOM = 0,
		NORMAL = 1,
		SOFT = 2,
		DRUM = 3
	};
	inline std::string to_string(const HitSoundType& hit_sound_type);
	inline std::string to_string(const HitSampleType& hit_sample_type);

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

	struct HitSample
	{
		enum class SampleSetType : uint8_t
		{
			TIMING_POINTS, NORMAL, ADDITION
		};

		HitSampleType normal_set = HitSampleType::NO_CUSTOM;
		HitSampleType addition_set = HitSampleType::NO_CUSTOM;
		int index = 0;
		int volume = 0;
		std::string file_name;

		[[nodiscard]] std::string to_string() const;
		// -> <Bản chất, <Sử dụng của, Sử dụng cái gì>>
		[[nodiscard]] std::pair<SampleSetType, std::pair<SampleSetType, HitSampleType>>
			get_used_sample_set(const HitSoundType& hit_sound_type, const HitSampleType& current_timing_point_sample_type) const;

		HitSample() = default;
		explicit HitSample(const std::string& hitsample_str);
		size_t operator()(const HitSample& hs) const;
		HitSample& operator= (const OsuParser::Beatmap::Objects::HitObject::HitSample& hs);
	};
	inline std::string get_hit_sound_filename(const HitSoundType& hit_sound_type, const HitSample& hit_sample,
		const HitSampleType& current_timing_point_sample_type);
	inline std::vector<std::string> get_hitsound_filename(const HitSound& hit_sound, const HitSample& hit_sample, 
		const HitSampleType& current_timing_point_sample_type);
}