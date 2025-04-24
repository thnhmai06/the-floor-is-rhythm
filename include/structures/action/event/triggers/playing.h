#pragma once
#include <variant>
#include "structures/game/beatmap/hitsound.h"

namespace Structures::Action::Event::Triggers
{
	namespace Playing
	{
		struct HitSound
		{
			struct HitSoundCondition
			{
				static constexpr auto ALL = Game::Beatmap::Hitsound::HitSoundType::NORMAL;
				Game::Beatmap::Hitsound::HitSoundType condition = ALL;

				[[nodiscard]] bool is_satisfied(const Game::Beatmap::Hitsound::HitSound& hit_sound) const;

				HitSoundCondition() = default;
			};
			struct HitSampleCondition
			{
				static constexpr auto ALL_TYPE = Game::Beatmap::Hitsound::HitSample::SampleSetType::TIMING_POINTS;
				static constexpr auto ALL_CONDITION = Game::Beatmap::Hitsound::HitSampleType::NO_CUSTOM;

				Game::Beatmap::Hitsound::HitSample::SampleSetType type = ALL_TYPE;
				Game::Beatmap::Hitsound::HitSampleType condition = ALL_CONDITION;

				[[nodiscard]] bool is_satisfied(
					const Game::Beatmap::Hitsound::HitSoundType& hit_sound_type,
					const Game::Beatmap::Hitsound::HitSample& hit_sample,
					const Game::Beatmap::Hitsound::HitSampleType& current_timing_point_sample_type) const;
				[[nodiscard]] bool is_satisfied(
					const Game::Beatmap::Hitsound::HitSound& hit_sound,
					const Game::Beatmap::Hitsound::HitSample& hit_sample,
					const Game::Beatmap::Hitsound::HitSampleType& current_timing_point_sample_type) const;

				HitSampleCondition() = default;
				explicit HitSampleCondition(const Game::Beatmap::Hitsound::HitSample::SampleSetType& type);
			};
			struct IndexCondition
			{
				static constexpr int32_t ALL = -1;
				int32_t index = ALL;

				[[nodiscard]] bool is_satisfied(const Game::Beatmap::Hitsound::HitSample& hit_sample) const;

				IndexCondition() = default;
			};

			HitSoundCondition addition;
			HitSampleCondition sample_set{ Game::Beatmap::Hitsound::HitSample::SampleSetType::NORMAL };
			HitSampleCondition additions_sample_set{ Game::Beatmap::Hitsound::HitSample::SampleSetType::ADDITION };
			IndexCondition index; // custom_sample_set

			[[nodiscard]] bool is_satisfied(
				const Game::Beatmap::Hitsound::HitSound& hit_sound,
				const Game::Beatmap::Hitsound::HitSample& hit_sample,
				const Game::Beatmap::Hitsound::HitSampleType& current_timing_point_sample_type) const;
		};
		struct Passing
		{
		protected:

		public:
			[[nodiscard]] bool is_satisfied(const uint8_t& current_health) const;
		};
		struct Failing
		{
			[[nodiscard]] bool is_satisfied(const uint8_t& current_health) const;
		};

		using StoryboardTrigger = std::variant<HitSound, Passing, Failing>;
	}

}