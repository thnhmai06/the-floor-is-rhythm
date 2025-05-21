#include "engine/events/action/render.h" // Header
#include <utilities.h>
#include <utility>

namespace Engine::Events::Action::Render
{
	//! Fade
	// FadeAction
	void FadeAction::update(const double& value_percent, const double& from, const double& to)
	{
		target->alpha.percent = Utilities::Math::to_value(value_percent, from, to);
	}
	std::shared_ptr<Action> FadeAction::clone() const
	{
		return std::make_shared<FadeAction>(*this);
	}

	//! Move
	// MoveAction
	std::shared_ptr<Action> MoveAction::clone() const
	{
		return std::make_shared<MoveAction>(*this);
	}
	void MoveAction::update(const double& value_percent, const SDL_FPoint& from, const SDL_FPoint& to)
	{
		target->render_pos.x = Utilities::Math::to_value(value_percent, from.x, to.x);
		target->render_pos.y = Utilities::Math::to_value(value_percent, from.y, to.y);
	}

	// MoveXAction
	std::shared_ptr<Action> MoveXAction::clone() const
	{
		return std::make_shared<MoveXAction>(*this);
	}
	void MoveXAction::update(const double& value_percent, const float& from, const float& to)
	{
		target->render_pos.x = Utilities::Math::to_value(value_percent, from, to);
	}

	// MoveYAction
	std::shared_ptr<Action> MoveYAction::clone() const
	{
		return std::make_shared<MoveYAction>(*this);
	}
	void MoveYAction::update(const double& value_percent, const float& from, const float& to)
	{
		target->render_pos.y = Utilities::Math::to_value(value_percent, from, to);
	}

	//! Scale
	std::shared_ptr<Action> ScaleAction::clone() const
	{
		return std::make_shared<ScaleAction>(*this);
	}
	void ScaleAction::update(const double& value_percent, const SDL_FPoint& from, const SDL_FPoint& to)
	{
		if (const auto it = dynamic_cast<Engine::Render::Config::ObjectConfig*>(target))
		{
			it->scale = {
				.x = Utilities::Math::to_value(value_percent, from.x, to.x),
				.y = Utilities::Math::to_value(value_percent, from.y, to.y) };
		}
	}

	//! Rotate
	std::shared_ptr<Action> RotateAction::clone() const
	{
		return std::make_shared<RotateAction>(*this);
	}
	void RotateAction::update(const double& value_percent, const double& from, const double& to)
	{
		if (const auto it = dynamic_cast<Engine::Render::Config::ObjectConfig*>(target))
		{
			it->angle = Utilities::Math::to_value(value_percent, from, to);
		}
	}

	//! Color
	std::shared_ptr<Action> ColorAction::clone() const
	{
		return std::make_shared<ColorAction>(*this);
	}
	void ColorAction::update(const double& value_percent, const Color& from, const Color& to)
	{
		if (const auto it = dynamic_cast<Engine::Render::Config::ObjectConfig*>(target))
		{
			it->color.r = Utilities::Math::to_value(value_percent, from.r, to.r);
			it->color.g = Utilities::Math::to_value(value_percent, from.g, to.g);
			it->color.b = Utilities::Math::to_value(value_percent, from.b, to.b);
		}
	}

	//! Parameter
	std::shared_ptr<Action> ParameterAction::clone() const
	{
		return std::make_shared<ParameterAction>(*this);
	}
	void ParameterAction::reset_to_previous() const
	{
		if (const auto [time_start, time_end] = get_sequence_time();
			time_start == time_end) return; // không reset, vì nếu ko có endTime thì coi như là Vĩnh viễn
		if (const auto it = dynamic_cast<Engine::Render::Config::ObjectConfig*>(target))
		{
			switch (get_current_value().second)
			{
			case Parameter::Horizontal:
				it->flip_mode.horizontal = false;
				break;
			case Parameter::Vertical:
				it->flip_mode.vertical = false;
				break;
			case Parameter::AdditiveColour:
				it->blend_mode = SDL_BLENDMODE_BLEND;
				break;
			}
		}
	}
	void ParameterAction::update(const double& value_percent, const Parameter& from, const Parameter& to)
	{
		if (const auto it = dynamic_cast<Engine::Render::Config::ObjectConfig*>(target))
		{
			switch (to)
			{
			case Parameter::Horizontal:
				it->flip_mode.horizontal = true;
				break;
			case Parameter::Vertical:
				it->flip_mode.vertical = true;
				break;
			case Parameter::AdditiveColour:
				it->blend_mode = SDL_BLENDMODE_ADD;
				break;
			}
		}
	}
	void ParameterAction::on_next_sequence() { reset_to_previous(); }
	void ParameterAction::on_finished() { reset_to_previous(); }
	ParameterAction::ParameterAction(
		const int64_t& start_time, const int64_t& end_time, const Easing& easing,
		Engine::Render::Config::ObjectConfig& target, const Parameter& value,
		const std::vector<Parameter>& sequence) : RenderAction(start_time, end_time, easing, target, value, value, sequence)
	{
	}
}
