#pragma once
#include "structures/events/event/event.h"
#include "structures/game/mapset.h"

namespace Structures::Events::Event
{
	namespace Playing
	{
		struct Hitsound : Event
		{
			const Game::Beatmap::Hitsound::HitSound* hit_sound;
			const Game::Beatmap::Hitsound::HitSample* hit_sample;
			std::vector<std::pair<Types::Game::HitSound::SampleSetType, Types::Game::HitSound::HitSampleType>> used_sample_set;
			std::vector<std::string> used_audio;

			Types::Event::EventID get_id() noexcept override;

			explicit Hitsound(const Game::Beatmap::HitObjects::Floor& floor, const Game::Beatmap::Mapset& mapset, const Audio::EffectMemory& beatmap_effect);
		};
		struct Passing : Event
		{
			Types::Event::EventID get_id() noexcept override;
		};
		struct Failing : Event
		{
			Types::Event::EventID get_id() noexcept override;
		};
		struct Scoring : Event
		{
			const Game::Beatmap::HitObjects::Floor* floor;
			Types::Game::Gameplay::NoteScore score;

			Types::Event::EventID get_id() noexcept override;

			explicit Scoring(const Game::Beatmap::HitObjects::Floor* floor, const Types::Game::Gameplay::NoteScore& score);
		};
	}
}