#include "structures/action/event/callback/render.h" // Header
#include <utilities.hpp>

namespace Structures::Action::Event::Render
{
	//! BASE
	float RenderCallback::get_current_value_in_percent(const int64_t& current_time) const
	{
		return static_cast<float>(getEasingFunction(easing)(Utilities::Math::to_percent(current_time, start_time, end_time)));
	}
	bool RenderCallback::is_active(const int64_t& current_time)
	{
		return target_object && Utilities::Math::in_range(start_time, end_time, current_time);
	}

	//! Fade
	// FadeCallback
	void FadeCallback::execute(const int64_t& current_time)
	{
		if (!is_active(current_time)) return;
		const auto current_value_in_percent = get_current_value_in_percent(current_time);

		target_object->config.alpha = Utilities::Math::to_value(current_value_in_percent, from, to);
	}
	FadeCallback::FadeCallback(const easing_functions& easing, const float& from_percent, const float& to_percent)
		: from(Utilities::Math::to_value(from_percent, static_cast<uint8_t>(0), Structures::Render::Objects::SDL_MAX_ALPHA)),
		to(Utilities::Math::to_value(to_percent, static_cast<uint8_t>(0), Structures::Render::Objects::SDL_MAX_ALPHA))
	{
		this->easing = easing;
	}

	//! Move
	// MoveCallback
	void MoveCallback::execute(const int64_t& current_time)
	{
		if (!is_active(current_time)) return;
		const auto current_value_in_percent = get_current_value_in_percent(current_time);

		target_object->config.render_pos.x = Utilities::Math::to_value(current_value_in_percent, from.x, to.x);
		target_object->config.render_pos.y = Utilities::Math::to_value(current_value_in_percent, from.y, to.y);
	}
	// MoveXCallback
	void MoveXCallback::execute(const int64_t& current_time)
	{
		if (!is_active(current_time)) return;
		const auto current_value_in_percent = get_current_value_in_percent(current_time);

		target_object->config.render_pos.x = Utilities::Math::to_value(current_value_in_percent, from_x, to_x);
	}
	// MoveYCallback
	void MoveYCallback::execute(const int64_t& current_time)
	{
		if (!is_active(current_time)) return;
		const auto current_value_in_percent = get_current_value_in_percent(current_time);

		target_object->config.render_pos.y = Utilities::Math::to_value(current_value_in_percent, from_y, to_y);
	}

	//! Scale
	void ScaleCallback::execute(const int64_t& current_time)
	{
		if (!is_active(current_time)) return;
		const auto current_value_in_percent = get_current_value_in_percent(current_time);

		target_object->config.set_scale({
			Utilities::Math::to_value(current_value_in_percent, from.x, to.x),
			Utilities::Math::to_value(current_value_in_percent, from.y, to.y) });
	}
	ScaleCallback::ScaleCallback(const easing_functions& easing, const float& from, const float& to) :
		from{ from, from }, to{ to, to }
	{
		this->easing = easing;
	}

	//! Rotation
	void RotateCallback::execute(const int64_t& current_time)
	{
		if (!is_active(current_time)) return;
		const auto current_value_in_percent = get_current_value_in_percent(current_time);

		target_object->config.angle = Utilities::Math::to_value(current_value_in_percent, from, to);
	}

}
