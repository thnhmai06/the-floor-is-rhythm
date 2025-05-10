#pragma once
#include "structures/events/time/easing.hpp"
#include "structures/events/action/action.h"
#include "structures/render/object.h"
#include "structures/type.hpp"
#include "utilities.hpp"

namespace Structures::Events::Action::Render
{
	// trying to remake https://osu.ppy.sh/wiki/en/Storyboard/Scripting/Commands

	//! BASE
	template <typename ValueType>
	struct RenderAction : Action {
	private:
		[[nodiscard]] double get_current_value_percent(const int64_t& current_time) const
		{
			const auto [start, end] = get_sequence_time();

			return Time::Easing::get_easing_function(easing)(
				Utilities::Math::to_percent(current_time, start, end));
		}
		bool next_sequence()
		{
			if (current_sequence >= static_cast<long>(sequence.size()) - 1) return false;

			++current_sequence;
			on_next_sequence();
			return true;
		}

	protected:
		virtual void on_started() {}
		virtual void on_next_sequence() {}
		virtual void on_finished() {}
		virtual void update(const float& value_percent, const ValueType& from, const ValueType& to) {}

		void work(const int64_t& current_time)
		{
			if (!is_valid(current_time)) return;
			if (!started) 
			{
				started = true;
				on_started();
			}
			const auto value_percent = get_current_value_percent(current_time);
			const auto [crr_from, crr_to] = get_current_value();
			update(value_percent, crr_from, crr_to);
		}

	public:
		static constexpr long DEFAULT_SEQUENCE = -1;

		ValueType from, to;
		std::weak_ptr<Structures::Render::Object::Object> target_object;
		Time::Easing::EasingFunctionType easing;
		std::vector<ValueType> sequence;
		long current_sequence = DEFAULT_SEQUENCE;

		RenderAction(const int64_t& start_time,
			const int64_t& end_time,
			const Time::Easing::EasingFunctionType& easing,
			std::weak_ptr<Structures::Render::Object::Object> target_object,
			const ValueType& from,
			const ValueType& to,
			const std::vector<ValueType>& sequence = {})
			: from(from), to(to), target_object(std::move(target_object)), easing(easing), sequence(sequence)
		{
			this->start_time = start_time;
			const auto base_end = end_time;
			const auto base_duration = base_end - start_time;
			const auto n = static_cast<int64_t>(sequence.size());
			this->end_time = start_time + base_duration * (n + 1);
		}
		RenderAction() = default;

		[[nodiscard]] std::shared_ptr<Action> clone() const override
		{
			return std::make_shared<RenderAction>(*this);
		}

		void execute(const int64_t& current_time) final
		{
			if (auto sequence_time = get_sequence_time(); 
				current_time <= sequence_time.second)
				work(current_time);
			else {
				while (sequence_time.second < current_time)
				{
					if (next_sequence()) 
					{
						sequence_time = get_sequence_time();
						work(current_time);
					} else
					{
						work(current_time);
						finished = true;
						on_finished();
						return;
					}
					
				}
			}
		}

		[[nodiscard]] bool is_valid(const int64_t& current_time) const override
		{
			return Action::is_valid(current_time) && !target_object.expired() && target_object.lock();
		}

		[[nodiscard]] std::pair<int64_t, int64_t> get_sequence_time() const
		{
			const auto n = static_cast<long>(sequence.size());
			if (n == 0) return { start_time, end_time };

			const auto one_sequence_duration = (end_time - start_time) / (n + 1);
			const auto seg_index = std::clamp(current_sequence + 1, 0L, n);
			return { start_time + one_sequence_duration * seg_index, start_time + one_sequence_duration * (seg_index + 1) };
		}

		[[nodiscard]] std::pair<ValueType, ValueType> get_current_value()
		{
			const auto n = static_cast<long>(sequence.size());

			if (current_sequence < 0)
				return { from, to };
			if (current_sequence == 0)
				return { to, sequence[0] };
			if (current_sequence == 1)
				return { sequence[0], sequence[1] };
			if (current_sequence >= n)
				return { sequence[n - 2], sequence[n - 1] };
			return { sequence[current_sequence - 1], sequence[current_sequence] };
		}

	};

	//! Fade
	struct FadeAction : RenderAction<uint8_t>
	{
		void update(const float& value_percent, const uint8_t& from, const uint8_t& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
		FadeAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::FadeCommand& osu_fade);
	};

	//! Move
	struct MoveAction : RenderAction<SDL_FPoint>
	{
		void update(const float& value_percent, const SDL_FPoint& from, const SDL_FPoint& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
		MoveAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveCommand& osu_move);
	};
	struct MoveXAction : RenderAction<float>
	{
		void update(const float& value_percent, const float& from, const float& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
		MoveXAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveXCommand& osu_move_x);
	};
	struct MoveYAction : RenderAction<float>
	{
		void update(const float& value_percent, const float& from, const float& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
		MoveYAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveYCommand& osu_move_y);
	};

	//! Scale
	struct ScaleAction : RenderAction<SDL_FPoint>
	{
		void update(const float& value_percent, const SDL_FPoint& from, const SDL_FPoint& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		// Normal Scale
		ScaleAction(const int64_t& start_time, const int64_t& end_time,
			const Time::Easing::EasingFunctionType& easing,
			std::weak_ptr<Structures::Render::Object::Object> target_object,
			const float& from, const float& to,
			const std::vector<float>& sequence = {});
		ScaleAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::ScaleCommand& osu_scale);
		// Vector Scale
		using RenderAction::RenderAction;
		ScaleAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::VectorScaleCommand& osu_vector_scale);
	};

	//! Rotate
	struct RotateAction : RenderAction<float> // độ
	{
		void update(const float& value_percent, const float& from, const float& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
		RotateAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::RotateCommand& osu_rotate);
	};

	//! Color
	using Color = Types::Render::Color;
	struct ColorAction : RenderAction<Color>
	{
		void update(const float& value_percent, const Color& from, const Color& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;

		using RenderAction::RenderAction;
		ColorAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::ColorCommand& osu_color);
	};

	//! Parameter
	using Parameter = OsuParser::Beatmap::Objects::Event::Type::Commands::Args::Parameter::Parameter;
	// Trong này from là không có ý nghĩa
	struct ParameterAction : RenderAction<Parameter>
	{
	protected:
		struct
		{
			SDL_FlipMode flip_mode = SDL_FLIP_NONE;
			SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND;
		} previous;
		void save_previous();
		void reset_to_previous() const;

	public:
		void update(const float& value_percent, const Parameter& from, const Parameter& to) override;
		[[nodiscard]] std::shared_ptr<Action> clone() const override;
		void on_next_sequence() override;
		void on_finished() override;
		void on_started() override;

		using RenderAction::RenderAction;
		ParameterAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::ParameterCommand& osu_parameter);
	};
}