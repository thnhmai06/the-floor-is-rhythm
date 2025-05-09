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
		Hitsound::Hitsound(const Game::Beatmap::HitObjects::Floor& floor, const Game::Beatmap::Mapset& mapset)
			: hit_sound(&floor.hit_sound), hit_sample(&floor.hit_sample)
		{
			time = static_cast<int64_t>(std::round(floor.time));

			const auto& timing_point = mapset.timing_points.data.lower_bound(floor.time)->second;
			used_sample_set = hit_sample->get_used_sample_set(*hit_sound, 
				timing_point.sample.set, mapset.general.sample_set);
			used_audio = Game::Beatmap::Hitsound::get_hit_sound_filename(*hit_sound, *hit_sample, timing_point.sample, mapset.general.sample_set);
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