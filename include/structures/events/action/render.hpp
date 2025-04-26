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
	struct RenderAction : Action
	{
	private:
		[[nodiscard]] float get_current_value_percent(const int64_t& current_time) const
		{
			return static_cast<float>(
				Time::Easing::get_easing_function(easing)
				(Utilities::Math::to_percent(current_time, get_time_in_this_sequence(start_time), get_time_in_this_sequence(end_time))));
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
			const auto [crr_from, crr_to] = get_value_in_this_sequence();
			update(value_percent, crr_from, crr_to);
		}

	public:
		static constexpr long DEFAULT_SEQUENCE = -1;

		ValueType from, to;
		Structures::Render::Object::Object* target_object = nullptr;
		Time::Easing::EasingFunctionType easing;
		std::vector<ValueType> sequence;
		long current_sequence = DEFAULT_SEQUENCE;

		void execute(const int64_t& current_time) final
		{
			if (current_time <= get_time_in_this_sequence(end_time))
				work(current_time);
			else
				while (get_time_in_this_sequence(end_time) < current_time)
				{
					if (!next_sequence())
					{
						work(get_time_in_this_sequence(end_time));
						finished = true;
						on_finished();
						return;
					}
					work(current_time);
				}
		}
		[[nodiscard]] bool is_valid(const int64_t& current_time) const override
		{
			return !finished && target_object && get_time_in_this_sequence(start_time) <= current_time;
		}
		[[nodiscard]] int64_t get_time_in_this_sequence(const int64_t& time) const
		{
			const auto duration = end_time - start_time;
			return time + duration * std::clamp(current_sequence + 1, 0L, static_cast<long>(sequence.size()));
		}
		// [from, to]
		[[nodiscard]] std::pair<ValueType, ValueType> get_value_in_this_sequence()
		{
			if (sequence.empty() || current_sequence < 0) return { from, to };
			const auto n = sequence.size();
			if (sequence.size() <= 1 || current_sequence < 1) return { to, sequence.front() };

			if (current_sequence >= n) return { sequence[n - 2], sequence.back() };
			return { sequence[current_sequence - 1], sequence[current_sequence] };
		}

		RenderAction() = default;
		RenderAction(const int64_t& start_time, const int64_t& end_time,
			const Time::Easing::EasingFunctionType& easing,
			Structures::Render::Object::Object* target_object,
			const ValueType& from, const ValueType& to,
			const std::vector<ValueType>& sequence = {})
			: target_object(target_object)
		{
			this->start_time = start_time;
			this->end_time = end_time;
			this->easing = easing;
			this->from = from;
			this->to = to;
			this->sequence = sequence;
		}
	};

	//! Fade
	struct FadeAction : RenderAction<uint8_t>
	{
		void update(const float& value_percent, const uint8_t& from, const uint8_t& to) override;

		using RenderAction::RenderAction;
		FadeAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::FadeCommand& osu_fade);
	};

	//! Move
	struct MoveAction : RenderAction<SDL_FPoint>
	{
		void update(const float& value_percent, const SDL_FPoint& from, const SDL_FPoint& to) override;

		using RenderAction::RenderAction;
		MoveAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveCommand& osu_move);
	};
	struct MoveXAction : RenderAction<float>
	{
		void update(const float& value_percent, const float& from, const float& to) override;

		using RenderAction::RenderAction;
		MoveXAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveXCommand& osu_move_x);
	};
	struct MoveYAction : RenderAction<float>
	{
		void update(const float& value_percent, const float& from, const float& to) override;

		using RenderAction::RenderAction;
		MoveYAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveYCommand& osu_move_y);
	};

	//! Scale
	struct ScaleAction : RenderAction<SDL_FPoint>
	{
		void update(const float& value_percent, const SDL_FPoint& from, const SDL_FPoint& to) override;

		// Normal Scale
		ScaleAction(const int64_t& start_time, const int64_t& end_time,
			const Time::Easing::EasingFunctionType& easing,
			Structures::Render::Object::Object* target_object,
			const float& from, const float& to,
			const std::vector<float>& sequence = {});
		ScaleAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::ScaleCommand& osu_scale);
		// Vector Scale
		using RenderAction::RenderAction;
		ScaleAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::VectorScaleCommand& osu_vector_scale);
	};

	//! Rotate
	struct RotateAction : RenderAction<float> // độ
	{
		void update(const float& value_percent, const float& from, const float& to) override;

		using RenderAction::RenderAction;
		RotateAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::RotateCommand& osu_rotate);
	};

	//! Color
	using Color = Types::Render::Color;
	struct ColorAction : RenderAction<Color>
	{
		void update(const float& value_percent, const Color& from, const Color& to) override;

		using RenderAction::RenderAction;
		ColorAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::ColorCommand& osu_color);
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
		void on_next_sequence() override;
		void on_finished() override;
		void on_started() override;

		using RenderAction::RenderAction;
		ParameterAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::ParameterCommand& osu_parameter);
	};
}