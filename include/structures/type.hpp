#pragma once
#include <concepts>
#include <SDL3_mixer/SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <osu!parser/Parser/Structures/Beatmap/Objects/TimingPoint.hpp>
#include <osu!parser/Parser/Structures/Beatmap/Objects/Event.hpp>

namespace Structures::Types
{
	namespace Audio
	{
		using Music = Mix_Music*;
		using Effect = Mix_Chunk*;

		template <typename T>
		concept AudioPtrType = std::same_as<T, Music> || std::same_as<T, Effect>;
	}
	namespace Render
	{
		enum class LoopType : bool
		{
			LoopForever = false,
			LoopOnce = true
		};
		enum class OriginType : std::uint8_t
		{
			TopLeft = 0, Centre = 1, CentreLeft = 2, TopRight = 3,
			BottomCentre = 4, TopCentre = 5, Custom = 6,
			CentreRight = 7, BottomLeft = 8, BottomRight = 9
		};

		struct Color { uint8_t r, g, b; }; // giống SDL_Color nhưng bỏ alpha
	}
	namespace Event
	{
		enum class EventID : uint8_t
		{
			None,
			Hitsound,
			Passing,
			Failing,
		};
	}
	namespace Game
	{
		namespace HitSound
		{
			enum class HitSoundType : std::uint8_t
			{
				Normal = 0,
				Whistle = 1,
				Finish = 2,
				Clap = 3
			};
			enum class HitSampleType : std::uint8_t
			{
				NoCustom = 0,
				Normal = 1,
				Soft = 2,
				Drum = 3
			};
			enum class SampleSetType : uint8_t
			{
				Normal,
				Addition
			};
			inline HitSampleType to_floor_hit_sample(const OsuParser::Beatmap::Objects::TimingPoint::HitSampleType& osu_hitsample)
			{
				switch (osu_hitsample)
				{
				case OsuParser::Beatmap::Objects::TimingPoint::HitSampleType::NORMAL:
					return HitSampleType::Normal;
				case OsuParser::Beatmap::Objects::TimingPoint::HitSampleType::SOFT:
					return HitSampleType::Soft;
				case OsuParser::Beatmap::Objects::TimingPoint::HitSampleType::DRUM:
					return HitSampleType::Drum;
				case OsuParser::Beatmap::Objects::TimingPoint::HitSampleType::NO_CUSTOM:
					return HitSampleType::NoCustom;
				}
				return HitSampleType::Normal;
			}
			inline HitSampleType string_to_hit_sample(std::string str)
			{
				for (auto& c : str)
					c = std::tolower(c);
				if (str == "normal")
					return HitSampleType::Normal;
				if (str == "soft")
					return HitSampleType::Soft;
				if (str == "drum")
					return HitSampleType::Drum;
				return HitSampleType::NoCustom; // CHÚ Ý PHẢI THEO ALL TRONG EVENT CONDITION
			}
			inline HitSoundType string_to_hit_sound(std::string str)
			{
				for (auto& c : str)
					c = std::tolower(c);
				if (str == "whistle")
					return HitSoundType::Whistle;
				if (str == "finish")
					return HitSoundType::Finish;
				if (str == "clap")
					return HitSoundType::Clap;
				return HitSoundType::Normal; // CHÚ Ý PHẢI THEO ALL TRONG EVENT CONDITION
			}
			inline std::string to_string(const HitSoundType& hit_sound_type)
			{
				std::string result;
				switch (hit_sound_type)
				{
				case HitSoundType::Normal:
					result = "normal";
					break;
				case HitSoundType::Whistle:
					result = "whistle";
					break;
				case HitSoundType::Finish:
					result = "finish";
					break;
				case HitSoundType::Clap:
					result = "clap";
					break;
				}
				return result;
			}
			inline std::string to_string(const HitSampleType& hit_sample_type)
			{
				std::string result;
				switch (hit_sample_type)
				{
				case HitSampleType::NoCustom:
					result = "";
					break;
				case HitSampleType::Normal:
					result = "normal";
					break;
				case HitSampleType::Soft:
					result = "soft";
					break;
				case HitSampleType::Drum:
					result = "drum";
					break;
				}
				return result;
			}
		}
		namespace Gameplay
		{
			enum struct NoteScore : int16_t
			{
				Perfect = 300,
				Good = 100,
				Bad = 50,
				Miss = 0,
				Skip = -1
			};

			enum class PlayState : bool
			{
				Fail = false,
				Pass = true
			};

			enum class Layer : uint8_t
			{
				Background,
				Fail,
				Pass,
				Foreground
			};
		}
	}
	namespace Format
	{
		namespace Skin
		{
			using Alphabet = std::unordered_map<char, std::string>;
		}
	}
}
