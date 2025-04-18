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

		target_object->config.color.a = Utilities::Math::to_value(current_value_in_percent, from, to);
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

	//! Color
	void ColorCallback::execute(const int64_t& current_time)
	{
		if (!is_active(current_time)) return;
		const auto current_value_in_percent = get_current_value_in_percent(current_time);

		target_object->config.color.r = Utilities::Math::to_value(current_value_in_percent, from.r, to.r);
		target_object->config.color.g = Utilities::Math::to_value(current_value_in_percent, from.g, to.g);
		target_object->config.color.b = Utilities::Math::to_value(current_value_in_percent, from.b, to.b);
	}

	//! Parameter
	void ParameterCallback::reset_to_previous() const
	{
		if (!target_object) return;

		target_object->config.blend_mode = previous.blend_mode;
		target_object->config.flip_mode = previous.flip_mode;
	}
	void ParameterCallback::execute(const int64_t& current_time)
	{
		if (in_active)
		{
			if (current_time >= end_time)
			{
				in_active = false;
				return reset_to_previous();
			}
		}
		else
		{
			if (!is_active(current_time)) return;

			in_active = true;
			previous.blend_mode = target_object->config.blend_mode;
			previous.flip_mode = target_object->config.flip_mode;
			switch (parameter)
			{
			case Parser::Beatmap::Event::Type::Command::Parameter::Parameter::AdditiveColour:
				target_object->config.blend_mode = SDL_BLENDMODE_ADD;
				break;
			case Parser::Beatmap::Event::Type::Command::Parameter::Parameter::Horizontal:
				target_object->config.flip_mode = SDL_FLIP_HORIZONTAL;
				break;
			case Parser::Beatmap::Event::Type::Command::Parameter::Parameter::Vertical:
				target_object->config.flip_mode = SDL_FLIP_VERTICAL;
				break;
			}
		}

	}
	ParameterCallback::ParameterCallback(const char& parameter) :
		previous(), parameter(static_cast<Parser::Beatmap::Event::Type::Command::Parameter::Parameter>(parameter))
	{
	}

	//! Loop
	void LoopCallback::execute(const int64_t& current_time)
	{
		if (!is_active(current_time)) return;
		const auto current_relative_time = current_time - start_time;

		for (uint32_t time = 1; time <= loop_count; time++)
			for (const auto& callback : callbacks)
				if (callback)
					callback->execute(current_relative_time);
	}
}