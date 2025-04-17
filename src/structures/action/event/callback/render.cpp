#include "structures/action/event/callback/render.h" // Header
#include <utilities.hpp>

namespace Structures::Action::Event::Render
{
	// BASE
	bool RenderCallback::is_active(const int64_t& current_time)
	{
		return target_object && Utilities::Math::in_range(start_time, end_time, current_time);
	}

	// FadeCallback
	void FadeCallback::execute(const int64_t& current_time)
	{
		//if (!is_active(current_time)) return;

		const auto current_value_in_percent = static_cast<float>(getEasingFunction(easing)(
			Utilities::Math::to_percent(current_time, start_time, end_time)));
		target_object->config.alpha = Utilities::Math::to_value(current_value_in_percent, start_alpha, end_alpha);
	}

}