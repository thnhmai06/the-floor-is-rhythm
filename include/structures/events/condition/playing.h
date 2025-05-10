#pragma once
#include <regex>
#include <variant>
#include "structures/events/condition/condition.h"
#include "structures/game/mapset/hitsound.h"

namespace Structures::Events::Condition
{
	namespace Playing
	{
		struct HitSound : Condition
		{
			static const std::regex pattern;

			struct HitSoundCondition // aka Addition
			{
				static constexpr auto ALL = Game::Beatmap::Hitsound::HitSoundType::Normal;

				Game::Beatmap::Hitsound::HitSoundType condition = ALL;

				[[nodiscard]] bool is_satisfied(const Game::Beatmap::Hitsound::HitSound& hit_sound) const;
			};
			struct HitSampleCondition
			{
				static constexpr auto ALL_CONDITION = Game::Beatmap::Hitsound::HitSampleType::NoCustom;

				Game::Beatmap::Hitsound::HitSampleType normal = ALL_CONDITION;
				Game::Beatmap::Hitsound::HitSampleType addition = ALL_CONDITION;

				[[nodiscard]] bool is_satisfied(
					const std::vector<std::pair<Types::Game::HitSound::SampleSetType, Types::Game::HitSound::HitSampleType>>& sample_set_used) const;
			};
			struct IndexCondition
			{
				static constexpr int32_t ALL = -1;
				int32_t index = ALL;

				[[nodiscard]] bool is_satisfied(const Game::Beatmap::Hitsound::HitSample& hit_sample) const;

				IndexCondition() = default;
			};

			HitSoundCondition addition;
			HitSampleCondition sample_set; // sample_set + additions_sample_set
			IndexCondition index; // custom_sample_set

			[[nodiscard]] std::set<Types::Event::EventID> get_allowed_event() const override;
			[[nodiscard]] bool is_satisfied(
				const Game::Beatmap::Hitsound::HitSound& hit_sound,
				const Game::Beatmap::Hitsound::HitSample& hit_sample,
				const std::vector<std::pair<Types::Game::HitSound::SampleSetType, Types::Game::HitSound::HitSampleType>>& sample_set_used) const;
			bool is_satisfied(const Event::Event* event) const override;

			explicit HitSound(const std::string& trigger_str);
		};
		struct Passing : Condition
		{
			static const std::regex pattern;

			[[nodiscard]] std::set<Types::Event::EventID> get_allowed_event() const override;
			bool is_satisfied(const Event::Event* event) const override;
		};
		struct Failing : Condition
		{
			static const std::regex pattern;

			[[nodiscard]] std::set<Types::Event::EventID> get_allowed_event() const override;
			bool is_satisfied(const Event::Event* event) const override;
		};

		std::shared_ptr<Condition> make_condition(const std::string& trigger_str);
	}
}
