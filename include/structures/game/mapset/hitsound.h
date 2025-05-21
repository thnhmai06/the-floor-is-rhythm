#pragma once
#include <osu!parser/Parser/Structures/Beatmap/Objects/HitObject.hpp>
#include "engine/audio/memory.h"
#include "core/type.h"
#include "engine/structures.h"

// lưu trữ Additions và Hitsample giống như Osu!
// p/s: code trong Osu!parser/Convert/Structures/Playing/HitObjects.h
// phần Additions và HitSample là do mình viết :D

namespace Structures::Game::Beatmap::Hitsound
{
	using namespace Core::Type::Game::HitSound;

	struct Additions
	{
		bool normal = false;
		bool whistle = false;
		bool finish = false;
		bool clap = false;

		void read(const std::int32_t& hit_sound_int);
		[[nodiscard]] bool has_addition(const Addition& additions) const;
		[[nodiscard]] std::int32_t to_int() const;

		Additions() = default;
		explicit Additions(const std::int32_t& hit_sound_int);
		Additions& operator= (const OsuParser::Beatmap::Objects::HitObject::Additions& hs);
		size_t operator()(const Additions& hs) const;
	};

	struct TimingSample
	{
		SampleSet set = SampleSet::Normal;
		int32_t index = 0;
		Engine::PercentValue<uint8_t> volume{ 0, MIX_MAX_VOLUME, 1 };
	};

	struct HitSample
	{
		SampleSet normal_set = SampleSet::NoCustom;
		SampleSet addition_set = SampleSet::NoCustom;
		int32_t index = 0;
		Engine::PercentValue<uint8_t> volume{ 0, MIX_MAX_VOLUME, 0 };
		std::string file_name = {};

		[[nodiscard]] std::string to_string() const;
		// Có thể bạn đang nhầm với get_hit_sound_filename()?
		[[nodiscard]] std::pair<SampleSetType, SampleSet>
			get_used_sample_set(
				const Addition& addition,
				const SampleSet& timing_sample_type,
				const SampleSet& mapset_sample_type) const;
		[[nodiscard]] std::vector<std::pair<SampleSetType, SampleSet>>
			get_used_sample_set(
				const Additions& additions,
				const SampleSet& timing_sample_type,
				const SampleSet& mapset_sample_type) const;
		void read(const std::string& hitsample_str);

		explicit HitSample(
			const SampleSet& normal_set = SampleSet::NoCustom, 
			const SampleSet& addition_set = SampleSet::NoCustom, 
			const int32_t& index = 0, const double& volume = 0, 
			std::string file_name = {});
		explicit HitSample(const std::string& hitsample_str);
		HitSample& operator=(const OsuParser::Beatmap::Objects::HitObject::SliderSample& slide_hs);
		HitSample& operator=(const OsuParser::Beatmap::Objects::HitObject::HitSample& hs);
	};

	 std::string get_hit_sound_filename(const Addition& addition, const HitSample& hit_sample,
		const TimingSample& timing_sample, const SampleSet& mapset_sample, const Engine::Audio::Memory::EffectMemory& beatmap_effect);
	 std::vector<std::string> get_hit_sound_filename(const Additions& additions, const HitSample& hit_sample, 
		 const TimingSample& timing_sample, const SampleSet& mapset_sample, const Engine::Audio::Memory::EffectMemory& beatmap_effect);
	 double get_hit_sample_volume(const HitSample& hit_sample, const TimingSample& timing_sample);
	
}