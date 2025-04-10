#pragma once
#include <osu!parser/Parser/Structures/Beatmap/Objects/HitObject.hpp>

// lưu trữ Hitsound và Hitsample giống như osu!
// p/s: code trong osu!parser/Parser/Structures/Beatmap/HitObjects.hpp
// phần Hitsound và HitSample là do mình viết :D

namespace GameObjects::Hitsound
{
	enum class HitsoundBitmap : std::uint8_t
	{
		NORMAL = 1 << 0,
		WHISTLE = 1 << 1,
		FINISH = 1 << 2,
		CLAP = 1 << 3
	};
	bool operator& (const HitsoundBitmap& left, const HitsoundBitmap& right);
	enum class SampleSetType : std::uint8_t
	{
		NO_CUSTOM = 0,
		NORMAL = 1,
		SOFT = 2,
		DRUM = 3
	};

	struct Hitsound
	{
		bool normal = false;
		bool whistle = false;
		bool finish = false;
		bool clap = false;
		void read(std::int32_t HitSound);

		Hitsound() = default;
		Hitsound(const std::int32_t hitsound_id) { read(hitsound_id); }
		Hitsound& operator= (const Parser::Hitsound& hs);
		[[nodiscard]] std::int32_t to_int() const;
	};
	struct HitSample
	{
		SampleSetType normal_set = SampleSetType::NO_CUSTOM;
		SampleSetType addition_set = SampleSetType::NO_CUSTOM;
		int index = 0;
		int volume = 0;
		std::string file_name;

		HitSample() = default;
		HitSample(const std::string& hitsample_str);
		HitSample& operator= (const Parser::HitObject::HitSample& hs);
		[[nodiscard]] std::string to_string() const;
		std::string get_hitsound_filename(const HitsoundBitmap& HitsoundType);
	};
}