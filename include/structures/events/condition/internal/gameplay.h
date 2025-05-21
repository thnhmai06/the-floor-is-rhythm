#pragma once
#include <regex>
#include "engine/events/condition.h"
#include "structures/game/mapset/hitsound.h"

namespace Structures::Events::Condition::Internal
{
	using Engine::Events::Condition::Internal::Condition;

	namespace Gameplay
	{
		struct HitSound : Condition
		{
			static const std::regex pattern;

			struct HitSoundCondition // aka Addition
			{
				Game::Beatmap::Hitsound::Addition condition = Core::Type::Game::HitSound::ADDITION_ALL;

				[[nodiscard]] bool is_satisfied(const Game::Beatmap::Hitsound::Additions& additions) const;
			};
			struct HitSampleCondition
			{
				Game::Beatmap::Hitsound::SampleSet normal = Core::Type::Game::HitSound::SAMPLE_SET_ALL;
				Game::Beatmap::Hitsound::SampleSet addition = Core::Type::Game::HitSound::SAMPLE_SET_ALL;

				[[nodiscard]] bool is_satisfied(
					const std::vector<std::pair<Core::Type::Game::HitSound::SampleSetType, Core::Type::Game::HitSound::SampleSet>>& sample_set_used) const;
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

			[[nodiscard]] bool is_satisfied(
				const Game::Beatmap::Hitsound::Additions& additions,
				const Game::Beatmap::Hitsound::HitSample& hit_sample,
				const std::vector<std::pair<Core::Type::Game::HitSound::SampleSetType, 
				Core::Type::Game::HitSound::SampleSet>>& sample_set_used) const;
			[[nodiscard]] bool is_satisfied(const Engine::Events::Event::Internal::EventView& event) const override;

			explicit HitSound(const std::string& trigger_str);
		};
		struct Passing : Condition
		{
			static const std::regex pattern;

			Passing();
		};
		struct Failing : Condition
		{
			static const std::regex pattern;

			Failing();
		};

		std::shared_ptr<Condition> make_condition(const std::string& trigger_str);
	}
}
