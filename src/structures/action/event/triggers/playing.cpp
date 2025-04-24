#include "structures/action/event/triggers/playing.h" // Header

namespace Structures::Action::Event::Triggers
{
	namespace Playing
	{
		//! HitSound (conditions)
		// HitSoundCondition
		bool HitSound::HitSoundCondition::is_satisfied(const Game::Beatmap::Hitsound::HitSound& hit_sound) const
		{
			if (condition == ALL) return true;
			return hit_sound.is_hit_sound_type_played(condition);
		}
		// HitSampleCondition
		bool HitSound::HitSampleCondition::is_satisfied(
			const Game::Beatmap::Hitsound::HitSoundType& hit_sound_type,
			const Game::Beatmap::Hitsound::HitSample& hit_sample,
			const Game::Beatmap::Hitsound::HitSampleType& current_timing_point_sample_type) const
		{
			const auto [sample_set_type, used_sample_set] = hit_sample.get_used_sample_set(hit_sound_type, current_timing_point_sample_type);
			const bool type_satisfied = type == ALL_TYPE || type == sample_set_type;
			const bool condition_satisfied = condition == ALL_CONDITION || condition == used_sample_set.second;

			return type_satisfied && condition_satisfied;
		}
		bool HitSound::HitSampleCondition::is_satisfied(
			const Game::Beatmap::Hitsound::HitSound& hit_sound,
			const Game::Beatmap::Hitsound::HitSample& hit_sample,
			const Game::Beatmap::Hitsound::HitSampleType& current_timing_point_sample_type) const
		{
			const bool whistle_satisfied =
				hit_sound.is_hit_sound_type_played(Game::Beatmap::Hitsound::HitSoundType::WHISTLE)
				&& is_satisfied(Game::Beatmap::Hitsound::HitSoundType::WHISTLE, hit_sample, current_timing_point_sample_type);
			const bool finish_satisfied =
				hit_sound.is_hit_sound_type_played(Game::Beatmap::Hitsound::HitSoundType::FINISH)
				&& is_satisfied(Game::Beatmap::Hitsound::HitSoundType::FINISH, hit_sample, current_timing_point_sample_type);
			const bool clap_satisfied =
				hit_sound.is_hit_sound_type_played(Game::Beatmap::Hitsound::HitSoundType::CLAP)
				&& is_satisfied(Game::Beatmap::Hitsound::HitSoundType::CLAP, hit_sample, current_timing_point_sample_type);
			return whistle_satisfied || finish_satisfied || clap_satisfied;
		}
		HitSound::HitSampleCondition::HitSampleCondition(const Game::Beatmap::Hitsound::HitSample::SampleSetType& type): type(type)
		{
		}
		// IndexCondition
		bool HitSound::IndexCondition::is_satisfied(const Game::Beatmap::Hitsound::HitSample& hit_sample) const
		{
			return index == ALL || index == hit_sample.index;
		}
		//! HitSound
		bool HitSound::is_satisfied(
			const Game::Beatmap::Hitsound::HitSound& hit_sound,
			const Game::Beatmap::Hitsound::HitSample& hit_sample,
			const Game::Beatmap::Hitsound::HitSampleType& current_timing_point_sample_type) const
		{
			return
				addition.is_satisfied(hit_sound)
				&& sample_set.is_satisfied(hit_sound, hit_sample, current_timing_point_sample_type)
				&& additions_sample_set.is_satisfied(hit_sound, hit_sample, current_timing_point_sample_type)
				&& index.is_satisfied(hit_sample);
		}
	}
}