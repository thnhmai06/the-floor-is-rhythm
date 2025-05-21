#pragma once
#include "engine/audio/type.h"
#include "engine/audio/buses.h"
#include "engine/events/action.h"

namespace Engine::Events::Action
{
	namespace Audio
	{
		using namespace Engine::Audio;

		struct PlayEffectAction : Action
		{
			const Bus::Bus<Effect>* target;
			Memory::EffectMemory::Item sound;
			std::optional<double> volume;

			[[nodiscard]] std::shared_ptr<Action> clone() const override;
			void execute(const int64_t& current_time) override;

			explicit PlayEffectAction(
				const Bus::Bus<Effect>& target, const Timing::Time& time,
				Memory::EffectMemory::Item sound, std::optional<double> volume = std::nullopt);
		};
	}
}