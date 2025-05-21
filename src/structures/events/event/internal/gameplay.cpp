#include "structures/events/event/internal/gameplay.h" // Header

namespace Structures::Events::Event::Internal
{
	namespace Gameplay
	{
		//! Hitsound
		Hitsound::Hitsound(
			const Time& time,
			const Game::Beatmap::HitObjects::Floor& floor,
			const Game::Beatmap::Hitsound::TimingSample& timing_sample,
			const Game::Beatmap::Hitsound::SampleSet& mapset_sample,
			const Engine::Audio::Memory::EffectMemory& beatmap_effect)
			: Event(time, Core::Type::Event::Hitsound),
				additions(&floor.additions), hit_sample(&floor.hit_sample)
		{
			used_sample_set = hit_sample->get_used_sample_set(*additions, timing_sample.set, mapset_sample);
			used_audio = Game::Beatmap::Hitsound::get_hit_sound_filename(
				*additions, *hit_sample, timing_sample, mapset_sample, beatmap_effect);
		}

		//! Passing
		Passing::Passing(const Time& time)
			: Event(time, Core::Type::Event::Passing)
		{
		}

		//! Failing
		Failing::Failing(const Time& time)
			: Event(time, Core::Type::Event::Failing)
		{
		}

		//! ComboBreak
		ComboBreak::ComboBreak(const Time& time, const unsigned long& previous_combo)
			: Event(time, Core::Type::Event::ComboBreak), previous_combo(previous_combo)
		{
		}

		//! Scoring
		Scoring::Scoring(
			const Time& time,
			Game::Beatmap::HitObjects::HitObjects::Container::const_iterator object,
			const Core::Type::Game::Gameplay::NoteScore& score)
			: Event(time, Core::Type::Event::Scoring), object(std::move(object)), score(score)
		{
		}
	}
}