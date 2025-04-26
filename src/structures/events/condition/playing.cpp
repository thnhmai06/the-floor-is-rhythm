#include "structures/events/condition/playing.h" // Header
#include "structures/events/event/playing.h"

namespace Structures::Events::Condition
{
	namespace Playing
	{
		//! HitSound (sub-conditions)
		bool HitSound::HitSoundCondition::is_satisfied(const Game::Beatmap::Hitsound::HitSound& hit_sound) const
		{
			if (condition == ALL) return true;
			return hit_sound.is_hit_sound_type_played(condition);
		}
		bool HitSound::HitSampleCondition::is_satisfied(
			const std::vector<std::pair<Types::Game::HitSound::SampleSetType, Types::Game::HitSound::HitSampleType>>& sample_set_used) const
		{
			bool normal_satisfied = normal == ALL_CONDITION;
			bool addition_satisfied = addition == ALL_CONDITION;
			for (const auto & [type, sample] : sample_set_used)
			{
				if (type == Types::Game::HitSound::SampleSetType::Normal)
					normal_satisfied = normal_satisfied || (normal == sample);
				else if (type == Types::Game::HitSound::SampleSetType::Addition)
					addition_satisfied = addition_satisfied || (addition == sample);
			}

			return normal_satisfied && addition_satisfied;
		}
		bool HitSound::IndexCondition::is_satisfied(const Game::Beatmap::Hitsound::HitSample& hit_sample) const
		{
			return index == ALL || index == hit_sample.index;
		}

		//! HitSound (condition)
		const std::regex HitSound::pattern{ R"(^HitSound(?:(Normal|Soft|Drum|All))?(?:(Normal|Soft|Drum|All))?(Whistle|Finish|Clap)?(\d+)?$)" };
		std::set<Types::Event::EventID> HitSound::get_allowed_event() const
		{
			return { Types::Event::EventID::Hitsound };
		}
		bool HitSound::is_satisfied(
			const Game::Beatmap::Hitsound::HitSound& hit_sound,
			const Game::Beatmap::Hitsound::HitSample& hit_sample,
			const std::vector<std::pair<Types::Game::HitSound::SampleSetType, Types::Game::HitSound::HitSampleType>>& sample_set_used) const
		{
			return
				addition.is_satisfied(hit_sound)
				&& sample_set.is_satisfied(sample_set_used)
				&& index.is_satisfied(hit_sample);
		}

		bool HitSound::is_satisfied(const Event::Event* event) const
		{
			const auto& ptr = dynamic_cast<const Event::Playing::Hitsound*>(event);
			if (!ptr) return false;

			return is_satisfied(*ptr->hit_sound, *ptr->hit_sample, ptr->used_sample_set);
		}
		HitSound::HitSound(const std::string& trigger_str)
		{
			if (trigger_str.empty()) return;
			if (std::smatch matches;
				std::regex_match(trigger_str, matches, pattern))
			{
				if (matches.size() > 1 && matches[1].matched)
					sample_set.normal = Types::Game::HitSound::string_to_hit_sample(matches[1].str());
				if (matches.size() > 2 && matches[2].matched)
					sample_set.addition = Types::Game::HitSound::string_to_hit_sample(matches[2].str());
				if (matches.size() > 3 && matches[3].matched)
					addition.condition = Types::Game::HitSound::string_to_hit_sound(matches[3].str());
				if (matches.size() > 4 && matches[4].matched)
					index.index = std::stoi(matches[4].str());
			}
		}

		//! Passing
		const std::regex Passing::pattern{ R"(^Passing$)" };
		std::set<Types::Event::EventID> Passing::get_allowed_event() const
		{
			return { Types::Event::EventID::Passing };
		}
		bool Passing::is_satisfied(const Event::Event* event) const
		{
			return dynamic_cast<const Event::Playing::Passing*>(event);
		}


		//! Failing
		const std::regex Failing::pattern{ R"(^Failing$)" };
		std::set<Types::Event::EventID> Failing::get_allowed_event() const
		{
			return { Types::Event::EventID::Failing };
		}
		bool Failing::is_satisfied(const Event::Event* event) const
		{
			return dynamic_cast<const Event::Playing::Failing*>(event);
		}

		//! general
		std::unique_ptr<Condition> make_condition(const std::string& trigger_str)
		{
			if (std::regex_match(trigger_str, HitSound::pattern))
				return std::make_unique<HitSound>(trigger_str);
			if (std::regex_match(trigger_str, Passing::pattern))
				return std::make_unique<Passing>();
			if (std::regex_match(trigger_str, Failing::pattern))
				return std::make_unique<Failing>();
			return nullptr;
		}
	}
}
