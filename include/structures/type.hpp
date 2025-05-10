#pragma once
#include <concepts>
#include <easing.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <osu!parser/Parser/Structures/Beatmap/Objects/HitObject.hpp>
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
		using EasingFunctionType = OsuParser::Beatmap::Objects::Event::Type::Commands::Args::Easing::Easing;
		using EasingFunction = easingFunction;

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
			Scoring
		};
	}
	namespace Game
	{
		namespace HitSound
		{
			enum class Addition : std::uint8_t
			{
				Normal = 0,
				Whistle = 1,
				Finish = 2,
				Clap = 3
			};
			enum class SampleSet : std::uint8_t
			{
				NoCustom = 0,
				Normal = 1,
				Soft = 2,
				Drum = 3
			};
			enum class SampleSetType : bool
			{
				Normal,
				Addition
			};
			inline SampleSet string_to_sample_set(std::string str)
			{
				for (auto& c : str)
					c = std::tolower(c);
				if (str == "normal")
					return SampleSet::Normal;
				if (str == "soft")
					return SampleSet::Soft;
				if (str == "drum")
					return SampleSet::Drum;
				return SampleSet::NoCustom; // CHÚ Ý PHẢI THEO ALL TRONG EVENT CONDITION
			}
			inline Addition string_to_additions(std::string str)
			{
				for (auto& c : str)
					c = std::tolower(c);
				if (str == "whistle")
					return Addition::Whistle;
				if (str == "finish")
					return Addition::Finish;
				if (str == "clap")
					return Addition::Clap;
				return Addition::Normal; // CHÚ Ý PHẢI THEO ALL TRONG EVENT CONDITION
			}
			inline std::string to_string(const Addition& additions)
			{
				std::string result;
				switch (additions)
				{
				case Addition::Normal:
					result = "normal";
					break;
				case Addition::Whistle:
					result = "whistle";
					break;
				case Addition::Finish:
					result = "finish";
					break;
				case Addition::Clap:
					result = "clap";
					break;
				}
				return result;
			}
			inline std::string to_string(const SampleSet& sample_set)
			{
				std::string result;
				switch (sample_set)
				{
				case SampleSet::NoCustom:
					result = "";
					break;
				case SampleSet::Normal:
					result = "normal";
					break;
				case SampleSet::Soft:
					result = "soft";
					break;
				case SampleSet::Drum:
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
