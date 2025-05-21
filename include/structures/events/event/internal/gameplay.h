#pragma once
#include "structures/game/mapset.h"

namespace Structures::Events::Event::Internal
{
	using Engine::Events::Event::Internal::Event;
	using Engine::Events::Timing::Time;

	namespace Gameplay
	{
		struct Hitsound : Event
		{
			const Game::Beatmap::Hitsound::Additions* additions;
			const Game::Beatmap::Hitsound::HitSample* hit_sample;
			std::vector<std::pair<Core::Type::Game::HitSound::SampleSetType, Core::Type::Game::HitSound::SampleSet>> used_sample_set;
			std::vector<std::string> used_audio;

			explicit Hitsound(
				const Time& time,
				const Game::Beatmap::HitObjects::Floor& floor, 
				const Game::Beatmap::Hitsound::TimingSample& timing_sample,
				const Game::Beatmap::Hitsound::SampleSet& mapset_sample,
				const Engine::Audio::Memory::EffectMemory& beatmap_effect);
		};
		struct Passing : Event
		{
			explicit Passing(const Time& time);
		};
		struct Failing : Event
		{
			explicit Failing(const Time& time);
		};
		struct ComboBreak : Event
		{
			unsigned long previous_combo;

			explicit ComboBreak(const Time& time, const unsigned long& previous_combo);
		};
		struct Scoring : Event
		{
			Game::Beatmap::HitObjects::HitObjects::Container::const_iterator object;
			Core::Type::Game::Gameplay::NoteScore score;

			explicit Scoring(
				const Time& time,
				Game::Beatmap::HitObjects::HitObjects::Container::const_iterator object,
				const Core::Type::Game::Gameplay::NoteScore& score);
		};
	}
}