#include "structures/events/action/render.hpp" // Header
#include <utilities.hpp>

namespace Structures::Events::Action::Render
{
	//! Fade
	// FadeAction
	void FadeAction::update(const float& value_percent, const uint8_t& from, const uint8_t& to)
	{
		target_object->config.color.a = Utilities::Math::to_value(value_percent, from, to);
	}
	FadeAction::FadeAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::FadeCommand& osu_fade)
		: FadeAction(
			osu_fade.startTime, osu_fade.endTime, osu_fade.easing, target_object,
			Utilities::Math::to_value(osu_fade.startOpacity, static_cast<uint8_t>(0), Structures::Render::Object::SDL_MAX_ALPHA),
			Utilities::Math::to_value(osu_fade.endOpacity, static_cast<uint8_t>(0), Structures::Render::Object::SDL_MAX_ALPHA))
	{
		for (const auto& value : osu_fade.sequence)
			sequence.push_back(Utilities::Math::to_value(value, static_cast<uint8_t>(0), Structures::Render::Object::SDL_MAX_ALPHA));
	}

	//! Move
	// MoveAction
	void MoveAction::update(const float& value_percent, const SDL_FPoint& from, const SDL_FPoint& to)
	{
		target_object->config.render_pos.x = Utilities::Math::to_value(value_percent, from.x, to.x);
		target_object->config.render_pos.y = Utilities::Math::to_value(value_percent, from.y, to.y);
	}
	MoveAction::MoveAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveCommand& osu_move)
		: MoveAction(
			osu_move.startTime, osu_move.endTime, osu_move.easing, target_object,
			{ .x = static_cast<float>(osu_move.startX), .y = static_cast<float>(osu_move.startY) },
			{ .x = static_cast<float>(osu_move.endX), .y = static_cast<float>(osu_move.endY) })
	{
		for (const auto& [x, y] : osu_move.sequence)
			sequence.push_back(SDL_FPoint{ static_cast<float>(x), static_cast<float>(y) });
	}

	// MoveXAction
	void MoveXAction::update(const float& value_percent, const float& from, const float& to)
	{
		target_object->config.render_pos.x = Utilities::Math::to_value(value_percent, from, to);
	}
	MoveXAction::MoveXAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveXCommand& osu_move_x)
		: MoveXAction(
			osu_move_x.startTime, osu_move_x.endTime, osu_move_x.easing, target_object,
			static_cast<float>(osu_move_x.startX), static_cast<float>(osu_move_x.endX))
	{
		for (const auto& x : osu_move_x.sequence)
			sequence.push_back(static_cast<float>(x));
	}

	// MoveYAction
	void MoveYAction::update(const float& value_percent, const float& from, const float& to)
	{
		target_object->config.render_pos.y = Utilities::Math::to_value(value_percent, from, to);
	}
	MoveYAction::MoveYAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveYCommand& osu_move_y)
		: MoveYAction(
			osu_move_y.startTime, osu_move_y.endTime, osu_move_y.easing, target_object,
			static_cast<float>(osu_move_y.startY), static_cast<float>(osu_move_y.endY))
	{
		for (const auto& y : osu_move_y.sequence)
			sequence.push_back(static_cast<float>(y));
	}

	//! Scale
	void ScaleAction::update(const float& value_percent, const SDL_FPoint& from, const SDL_FPoint& to)
	{
		target_object->config.set_scale({
			Utilities::Math::to_value(value_percent, from.x, to.x),
			Utilities::Math::to_value(value_percent, from.y, to.y) });
	}
	ScaleAction::ScaleAction(const int64_t& start_time, const int64_t& end_time,
		const Time::Easing::EasingFunctionType& easing, 
		Structures::Render::Object::Object* target_object,
		const float& from, const float& to, const std::vector<float>& sequence)
		: RenderAction{ start_time, end_time, easing, target_object, {.x = from, .y = from }, {.x = to, .y = to } }
	{
		for (const auto& value : sequence)
			this->sequence.push_back({ .x = value, .y = value });
	}
	ScaleAction::ScaleAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::ScaleCommand& osu_scale)
		: ScaleAction(
			osu_scale.startTime, osu_scale.endTime, osu_scale.easing, target_object,
			{ .x = static_cast<float>(osu_scale.startScale), .y = static_cast<float>(osu_scale.startScale) },
			{ .x = static_cast<float>(osu_scale.endScale), .y = static_cast<float>(osu_scale.endScale) })
	{
		for (const auto& value : osu_scale.sequence)
			sequence.push_back({ .x = static_cast<float>(value), .y = static_cast<float>(value) });
	}

	ScaleAction::ScaleAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::VectorScaleCommand& osu_vector_scale)
		: ScaleAction(
			osu_vector_scale.startTime, osu_vector_scale.endTime, osu_vector_scale.easing, target_object,
			{ .x = static_cast<float>(osu_vector_scale.startXScale), .y = static_cast<float>(osu_vector_scale.startYScale) },
			{ .x = static_cast<float>(osu_vector_scale.endXScale), .y = static_cast<float>(osu_vector_scale.endYScale) })
	{
		for (const auto& [x, y] : osu_vector_scale.sequence)
			sequence.push_back({ .x = static_cast<float>(x), .y = static_cast<float>(y) });
	}

	//! Rotation
	void RotateAction::update(const float& value_percent, const float& from, const float& to)
	{
		target_object->config.angle = Utilities::Math::to_value(value_percent, from, to);
	}

	RotateAction::RotateAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::RotateCommand& osu_rotate)
		: RotateAction(
			osu_rotate.startTime, osu_rotate.endTime, osu_rotate.easing, target_object,
			Utilities::Math::radian_to_degree(osu_rotate.startRotate),
			Utilities::Math::radian_to_degree(osu_rotate.endRotate))
	{
		for (const auto& value : osu_rotate.sequence)
			sequence.push_back(Utilities::Math::radian_to_degree(value));
	}

	//! Color
	void ColorAction::update(const float& value_percent, const Color& from, const Color& to)
	{
		target_object->config.color.r = Utilities::Math::to_value(value_percent, from.r, to.r);
		target_object->config.color.g = Utilities::Math::to_value(value_percent, from.g, to.g);
		target_object->config.color.b = Utilities::Math::to_value(value_percent, from.b, to.b);
	}

	ColorAction::ColorAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::ColorCommand& osu_color)
		: ColorAction(
			osu_color.startTime, osu_color.endTime, osu_color.easing, target_object,
			{ .r = static_cast<uint8_t>(osu_color.startR), .g = static_cast<uint8_t>(osu_color.startG), .b = static_cast<uint8_t>(osu_color.startB) },
			{ .r = static_cast<uint8_t>(osu_color.endR), .g = static_cast<uint8_t>(osu_color.endG), .b = static_cast<uint8_t>(osu_color.endB) })
	{
		for (const auto& [r, g, b] : osu_color.sequence)
			sequence.push_back(Color{ .r = static_cast<uint8_t>(r), .g = static_cast<uint8_t>(g), .b = static_cast<uint8_t>(b) });
	}


	//! Parameter
	void ParameterAction::save_previous()
	{
		previous.blend_mode = target_object->config.blend_mode;
		previous.flip_mode = target_object->config.flip_mode;
	}
	void ParameterAction::reset_to_previous() const
	{
		if (!target_object) return;

		target_object->config.blend_mode = previous.blend_mode;
		target_object->config.flip_mode = previous.flip_mode;
	}
	void ParameterAction::update(const float& value_percent, const Parameter& from, const Parameter& to)
	{
		switch (to)
		{
		case Parameter::AdditiveColour:
			target_object->config.blend_mode = SDL_BLENDMODE_ADD;
			break;
		case Parameter::Horizontal:
			target_object->config.flip_mode = SDL_FLIP_HORIZONTAL;
			break;
		case Parameter::Vertical:
			target_object->config.flip_mode = SDL_FLIP_VERTICAL;
			break;
		}
	}
	void ParameterAction::on_next_sequence() { reset_to_previous(); }
	void ParameterAction::on_finished() { reset_to_previous(); }
	void ParameterAction::on_started() { save_previous(); }
	ParameterAction::ParameterAction(Structures::Render::Object::Object* target_object, const OsuParser::Beatmap::Objects::Event::Commands::ParameterCommand& osu_parameter)
		: ParameterAction(
			osu_parameter.startTime, osu_parameter.endTime, osu_parameter.easing, target_object,
			static_cast<Parameter>(osu_parameter.parameter), static_cast<Parameter>(osu_parameter.parameter), 
			osu_parameter.sequence)
	{
	}
}