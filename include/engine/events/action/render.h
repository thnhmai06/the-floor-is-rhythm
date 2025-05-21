#pragma once
#include "engine/events/action/base.h"
#include "engine/render/config.h"

namespace Engine::Events::Action::Render
{
	// trying to remake https://osu.ppy.sh/wiki/en/Storyboard/Scripting/Commands

	template <typename ValueType>
	struct RenderAction : ChangeValueAction<ValueType>
	{
		Engine::Render::Config::RenderConfig* target;

		RenderAction(const int64_t& start_time,
			const int64_t& end_time,
			const Easing& easing,
			Engine::Render::Config::RenderConfig& target,
			const ValueType& from,
			const ValueType& to,
			const std::vector<ValueType>& sequence = {})
		: ChangeValueAction<ValueType>(start_time, end_time, easing, nullptr, from, to, sequence), target(&target)
		{
		}
		[[nodiscard]] bool is_valid(const int64_t& current_time) const override
		{
			return Action::is_valid(current_time) && !target;  // NOLINT(bugprone-parent-virtual-call)
		}
		[[nodiscard]] std::shared_ptr<Action> clone() const override
		{
			return std::make_shared<RenderAction>(*this);
		}
	};

	//! Fade
	struct FadeAction : RenderAction<double>
	{
		void update(const double& value_percent, const double& from, const double& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
	};

	//! Move
	struct MoveAction : RenderAction<SDL_FPoint>
	{
		void update(const double& value_percent, const SDL_FPoint& from, const SDL_FPoint& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
	};
	struct MoveXAction : RenderAction<float>
	{
		void update(const double& value_percent, const float& from, const float& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
	};
	struct MoveYAction : RenderAction<float>
	{
		void update(const double& value_percent, const float& from, const float& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
	};

	//! Scale
	struct ScaleAction : RenderAction<SDL_FPoint>
	{
		void update(const double& value_percent, const SDL_FPoint& from, const SDL_FPoint& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
	};

	//! Rotate
	struct RotateAction : RenderAction<double> // độ
	{
		void update(const double& value_percent, const double& from, const double& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
	};

	//! Color
	using Color = Engine::Render::Color;
	struct ColorAction : RenderAction<Color>
	{
		void update(const double& value_percent, const Color& from, const Color& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
	};

	//! Parameter
	// Trong này from là không có ý nghĩa
	struct ParameterAction : RenderAction<Parameter>
	{
	protected:
		void reset_to_previous() const;

	public:
		void update(const double& value_percent, const Parameter& from, const Parameter& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;
		void on_next_sequence() override;
		void on_finished() override;

		explicit ParameterAction(
			const int64_t& start_time,
			const int64_t& end_time,
			const Easing& easing,
			Engine::Render::Config::ObjectConfig& target,
			const Parameter& value,
			const std::vector<Parameter>& sequence = {});
	};
}