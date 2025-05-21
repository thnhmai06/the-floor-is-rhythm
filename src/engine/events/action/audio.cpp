#include "engine/events/action/audio.h" // Header

namespace Engine::Events::Action
{
	namespace Audio
	{
		std::shared_ptr<Action> PlayEffectAction::clone() const
		{
			return std::make_shared<PlayEffectAction>(*this);
		}
		void PlayEffectAction::execute(const int64_t& current_time)
		{
			Action::execute(current_time);
			target->play(sound, volume);
			finished = true;
		}
		PlayEffectAction::PlayEffectAction(const Bus::Bus<Effect>& target, const Timing::Time& time,
			Memory::EffectMemory::Item sound, const std::optional<double> volume)
		: target(&target), sound(std::move(sound)), volume(volume)
		{
			start_time = end_time = time;
		}
	}
}