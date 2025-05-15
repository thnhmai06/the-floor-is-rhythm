#include "structures/events/event/playing.h" // Header

namespace Structures::Events::Event
{
	namespace Playing
	{
		//! Hitsound
		Types::Event::EventID Hitsound::get_id() noexcept
		{
			return Types::Event::EventID::Hitsound;
		}
		Hitsound::Hitsound(const Game::Beatmap::HitObjects::Floor& floor, const Game::Beatmap::Mapset& mapset, const Audio::EffectMemory& beatmap_effect)
			: hit_sound(&floor.hit_sound), hit_sample(&floor.hit_sample)
		{
			time = static_cast<int64_t>(std::round(floor.time));

			auto timing_point = mapset.timing_points.data.lower_bound(floor.time);
			if (timing_point == mapset.timing_points.data.end())
				timing_point = Utilities::Container::get_last_element_iterator(mapset.timing_points.data);
			used_sample_set = hit_sample->get_used_sample_set(*hit_sound, 
				timing_point->second.sample.set, mapset.general.sample_set);
			used_audio = Game::Beatmap::Hitsound::get_hit_sound_filename(*hit_sound, *hit_sample, 
				timing_point->second.sample, mapset.general.sample_set, beatmap_effect);
		}

		//! Passing
		Types::Event::EventID Passing::get_id() noexcept
		{
			return Types::Event::EventID::Passing;
		}

		//! Failing
		Types::Event::EventID Failing::get_id() noexcept
		{
			return Types::Event::EventID::Failing;
		}

		//! Scoring
		Types::Event::EventID Scoring::get_id() noexcept
		{
			return Types::Event::EventID::Scoring;
		}
		Scoring::Scoring(const Game::Beatmap::HitObjects::Floor* floor, const Types::Game::Gameplay::NoteScore& score)
			: floor(floor), score(score)
		{
			time = static_cast<int64_t>(std::round(floor->time));
		}
	}
}