#include "structures/events/condition/internal/gameplay.h" // Header
#include "structures/events/event/internal/gameplay.h"

namespace Structures::Events::Condition::Internal
{
	namespace Gameplay
	{
		//! Additions (sub-conditions)
		bool HitSound::HitSoundCondition::is_satisfied(const Game::Beatmap::Hitsound::Additions& additions) const
		{
			if (condition == Core::Type::Game::HitSound::ADDITION_ALL) return true;
			return additions.has_addition(condition);
		}
		bool HitSound::HitSampleCondition::is_satisfied(
			const std::vector<std::pair<Core::Type::Game::HitSound::SampleSetType, Core::Type::Game::HitSound::SampleSet>>& sample_set_used) const
		{
			bool normal_satisfied = normal == Core::Type::Game::HitSound::SAMPLE_SET_ALL;
			bool addition_satisfied = addition == Core::Type::Game::HitSound::SAMPLE_SET_ALL;
			for (const auto & [type, sample] : sample_set_used)
			{
				if (type == Core::Type::Game::HitSound::SampleSetType::Normal)
					normal_satisfied = normal_satisfied || (normal == sample);
				else if (type == Core::Type::Game::HitSound::SampleSetType::Addition)
					addition_satisfied = addition_satisfied || (addition == sample);
			}

			return normal_satisfied && addition_satisfied;
		}
		bool HitSound::IndexCondition::is_satisfied(const Game::Beatmap::Hitsound::HitSample& hit_sample) const
		{
			return index == ALL || index == hit_sample.index;
		}

		//! Additions (condition)
		const std::regex HitSound::pattern{ R"(^Additions(?:(Normal|Soft|Drum|All))?(?:(Normal|Soft|Drum|All))?(Whistle|Finish|Clap)?(\d+)?$)" };
		bool HitSound::is_satisfied(
			const Game::Beatmap::Hitsound::Additions& additions,
			const Game::Beatmap::Hitsound::HitSample& hit_sample,
			const std::vector<std::pair<Core::Type::Game::HitSound::SampleSetType, Core::Type::Game::HitSound::SampleSet>>& sample_set_used) const
		{
			return
				addition.is_satisfied(additions)
				&& sample_set.is_satisfied(sample_set_used)
				&& index.is_satisfied(hit_sample);
		}

		bool HitSound::is_satisfied(const Engine::Events::Event::Internal::EventView& event) const
		{
			if (!Condition::is_satisfied(event)) return false;

			const auto it = event.lock();
			const auto& ptr = std::dynamic_pointer_cast<const Event::Internal::Gameplay::Hitsound>(it);
			if (!ptr) return false;

			return is_satisfied(*ptr->additions, *ptr->hit_sample, ptr->used_sample_set);
		}
		HitSound::HitSound(const std::string& trigger_str)
		{
			allowed_types = { Core::Type::Event::Hitsound };
			if (trigger_str.empty()) return;
			if (std::smatch matches;
				std::regex_match(trigger_str, matches, pattern))
			{
				if (matches.size() > 1 && matches[1].matched)
					sample_set.normal = Core::Type::Game::HitSound::string_to_sample_set(matches[1].str());
				if (matches.size() > 2 && matches[2].matched)
					sample_set.addition = Core::Type::Game::HitSound::string_to_sample_set(matches[2].str());
				if (matches.size() > 3 && matches[3].matched)
					addition.condition = Core::Type::Game::HitSound::string_to_additions(matches[3].str());
				if (matches.size() > 4 && matches[4].matched)
					index.index = std::stoi(matches[4].str());
			}
		}

		//! Passing
		const std::regex Passing::pattern{ R"(^Passing$)" };
		Passing::Passing()
		{
			allowed_types = { Core::Type::Event::Passing };
		}

		//! Failing
		const std::regex Failing::pattern{ R"(^Failing$)" };
		Failing::Failing()
		{
			allowed_types = { Core::Type::Event::Failing };
		}

		//! general
		std::shared_ptr<Condition> make_condition(const std::string& trigger_str)
		{
			if (std::regex_match(trigger_str, HitSound::pattern))
				return std::make_shared<HitSound>(trigger_str);
			if (std::regex_match(trigger_str, Passing::pattern))
				return std::make_shared<Passing>();
			if (std::regex_match(trigger_str, Failing::pattern))
				return std::make_shared<Failing>();
			return nullptr;
		}
	}
}
