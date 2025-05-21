#pragma once
#include <string>
#include "engine/events/type.h"

namespace Core::Type
{
	namespace Event
	{
		using Engine::Events::Event::Internal::EventType;
		
		constexpr EventType None = 0;
		constexpr EventType Hitsound = 1;
		constexpr EventType Passing = 2;
		constexpr EventType Failing = 3;
		constexpr EventType Scoring = 4;
		constexpr EventType ComboBreak = 5;
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
			constexpr auto ADDITION_ALL = Addition::Normal;
			enum class SampleSet : std::uint8_t
			{
				NoCustom = 0,
				Normal = 1,
				Soft = 2,
				Drum = 3
			};
			constexpr auto SAMPLE_SET_ALL = SampleSet::NoCustom;
			enum class SampleSetType : bool
			{
				Normal,
				Addition
			};
			SampleSet string_to_sample_set(std::string str);
			Addition string_to_additions(std::string str);
			std::string to_string(const Addition& additions);
			std::string to_string(const SampleSet& sample_set);
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
		}
	}
}
