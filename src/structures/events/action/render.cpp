#include "structures/events/action/render.hpp" // Header
#include <utilities.hpp>
#include <utility>

namespace Structures::Events::Action::Render
{
	//! Fade
	// FadeAction
	void FadeAction::update(const float& value_percent, const uint8_t& from, const uint8_t& to)
	{
		const auto it = target_object.lock();
		it->config.color.a = Utilities::Math::to_value(value_percent, from, to);
	}
	std::shared_ptr<Action> FadeAction::clone() const
	{
		return std::make_shared<FadeAction>(*this);
	}
	FadeAction::FadeAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::FadeCommand& osu_fade)
		: FadeAction(
			osu_fade.startTime, osu_fade.endTime, osu_fade.easing, std::move(target_object),
			Utilities::Math::to_value(osu_fade.startOpacity, static_cast<uint8_t>(0), Structures::Render::Object::SDL_MAX_ALPHA),
			Utilities::Math::to_value(osu_fade.endOpacity, static_cast<uint8_t>(0), Structures::Render::Object::SDL_MAX_ALPHA))
	{
		for (const auto& value : osu_fade.sequence)
			sequence.push_back(Utilities::Math::to_value(value, static_cast<uint8_t>(0), Structures::Render::Object::SDL_MAX_ALPHA));
	}

	//! Move
	// MoveAction
	std::shared_ptr<Action> MoveAction::clone() const
	{
		return std::make_shared<MoveAction>(*this);
	}
	void MoveAction::update(const float& value_percent, const SDL_FPoint& from, const SDL_FPoint& to)
	{
		const auto it = target_object.lock();
		it->config.render_pos.x = Utilities::Math::to_value(value_percent, from.x, to.x);
		it->config.render_pos.y = Utilities::Math::to_value(value_percent, from.y, to.y);
	}
	MoveAction::MoveAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveCommand& osu_move)
		: MoveAction(
			osu_move.startTime, osu_move.endTime, osu_move.easing, std::move(target_object),
			{ .x = static_cast<float>(osu_move.startX), .y = static_cast<float>(osu_move.startY) },
			{ .x = static_cast<float>(osu_move.endX), .y = static_cast<float>(osu_move.endY) })
	{
		for (const auto& [x, y] : osu_move.sequence)
			sequence.push_back(SDL_FPoint{ static_cast<float>(x), static_cast<float>(y) });
	}

	// MoveXAction
	std::shared_ptr<Action> MoveXAction::clone() const
	{
		return std::make_shared<MoveXAction>(*this);
	}
	void MoveXAction::update(const float& value_percent, const float& from, const float& to)
	{
		const auto it = target_object.lock();
		it->config.render_pos.x = Utilities::Math::to_value(value_percent, from, to);
	}
	MoveXAction::MoveXAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveXCommand& osu_move_x)
		: MoveXAction(
			osu_move_x.startTime, osu_move_x.endTime, osu_move_x.easing, std::move(target_object),
			static_cast<float>(osu_move_x.startX), static_cast<float>(osu_move_x.endX))
	{
		for (const auto& x : osu_move_x.sequence)
			sequence.push_back(static_cast<float>(x));
	}

	// MoveYAction
	std::shared_ptr<Action> MoveYAction::clone() const
	{
		return std::make_shared<MoveYAction>(*this);
	}
	void MoveYAction::update(const float& value_percent, const float& from, const float& to)
	{
		const auto it = target_object.lock();
		it->config.render_pos.y = Utilities::Math::to_value(value_percent, from, to);
	}
	MoveYAction::MoveYAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::MoveYCommand& osu_move_y)
		: MoveYAction(
			osu_move_y.startTime, osu_move_y.endTime, osu_move_y.easing, std::move(target_object),
			static_cast<float>(osu_move_y.startY), static_cast<float>(osu_move_y.endY))
	{
		for (const auto& y : osu_move_y.sequence)
			sequence.push_back(static_cast<float>(y));
	}

	//! Scale
	std::shared_ptr<Action> ScaleAction::clone() const
	{
		return std::make_shared<ScaleAction>(*this);
	}
	void ScaleAction::update(const float& value_percent, const SDL_FPoint& from, const SDL_FPoint& to)
	{
		const auto it = target_object.lock();
		it->config.scale = {
			Utilities::Math::to_value(value_percent, from.x, to.x),
			Utilities::Math::to_value(value_percent, from.y, to.y) };
	}
	ScaleAction::ScaleAction(const int64_t& start_time, const int64_t& end_time,
		const Time::Easing::EasingFunctionType& easing,
		std::weak_ptr<Structures::Render::Object::Object> target_object,
		const float& from, const float& to, const std::vector<float>& sequence)
		: RenderAction{ start_time, end_time, easing, std::move(target_object), {.x = from, .y = from }, {.x = to, .y = to } }
	{
		const auto it = target_object.lock();
		for (const auto& value : sequence)
			this->sequence.push_back({ .x = value, .y = value });
	}
	ScaleAction::ScaleAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::ScaleCommand& osu_scale)
		: ScaleAction(
			osu_scale.startTime, osu_scale.endTime, osu_scale.easing, std::move(target_object),
			{ .x = static_cast<float>(osu_scale.startScale), .y = static_cast<float>(osu_scale.startScale) },
			{ .x = static_cast<float>(osu_scale.endScale), .y = static_cast<float>(osu_scale.endScale) })
	{
		for (const auto& value : osu_scale.sequence)
			sequence.push_back({ .x = static_cast<float>(value), .y = static_cast<float>(value) });
	}

	ScaleAction::ScaleAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::VectorScaleCommand& osu_vector_scale)
		: ScaleAction(
			osu_vector_scale.startTime, osu_vector_scale.endTime, osu_vector_scale.easing, std::move(target_object),
			{ .x = static_cast<float>(osu_vector_scale.startXScale), .y = static_cast<float>(osu_vector_scale.startYScale) },
			{ .x = static_cast<float>(osu_vector_scale.endXScale), .y = static_cast<float>(osu_vector_scale.endYScale) })
	{
		for (const auto& [x, y] : osu_vector_scale.sequence)
			sequence.push_back({ .x = static_cast<float>(x), .y = static_cast<float>(y) });
	}

	//! Rotate
	std::shared_ptr<Action> RotateAction::clone() const
	{
		return std::make_shared<RotateAction>(*this);
	}
	void RotateAction::update(const float& value_percent, const float& from, const float& to)
	{
		const auto it = target_object.lock();
		it->config.angle = Utilities::Math::to_value(value_percent, from, to);
	}

	RotateAction::RotateAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::RotateCommand& osu_rotate)
		: RotateAction(
			osu_rotate.startTime, osu_rotate.endTime, osu_rotate.easing, std::move(target_object),
			Utilities::Math::radian_to_degree(osu_rotate.startRotate),
			Utilities::Math::radian_to_degree(osu_rotate.endRotate))
	{
		for (const auto& value : osu_rotate.sequence)
			sequence.push_back(Utilities::Math::radian_to_degree(value));
	}

	//! Color
	std::shared_ptr<Action> ColorAction::clone() const
	{
		return std::make_shared<ColorAction>(*this);
	}
	void ColorAction::update(const float& value_percent, const Color& from, const Color& to)
	{
		const auto it = target_object.lock();
		it->config.color.r = Utilities::Math::to_value(value_percent, from.r, to.r);
		it->config.color.g = Utilities::Math::to_value(value_percent, from.g, to.g);
		it->config.color.b = Utilities::Math::to_value(value_percent, from.b, to.b);
	}

	ColorAction::ColorAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::ColorCommand& osu_color)
		: ColorAction(
			osu_color.startTime, osu_color.endTime, osu_color.easing, std::move(target_object),
			{ .r = static_cast<uint8_t>(osu_color.startR), .g = static_cast<uint8_t>(osu_color.startG), .b = static_cast<uint8_t>(osu_color.startB) },
			{ .r = static_cast<uint8_t>(osu_color.endR), .g = static_cast<uint8_t>(osu_color.endG), .b = static_cast<uint8_t>(osu_color.endB) })
	{
		for (const auto& [r, g, b] : osu_color.sequence)
			sequence.push_back(Color{ .r = static_cast<uint8_t>(r), .g = static_cast<uint8_t>(g), .b = static_cast<uint8_t>(b) });
	}


	//! Parameter
	std::shared_ptr<Action> ParameterAction::clone() const
	{
		return std::make_shared<ParameterAction>(*this);
	}
	void ParameterAction::reset_to_previous() const
	{
		if (target_object.expired()) return;
		const auto it = target_object.lock();
		if (!it) return;
		if (const auto [time_start, time_end] = get_sequence_time(); 
			time_start == time_end) return; // không reset, vì nếu ko có endTime thì coi như là Vĩnh viễn

		switch (get_current_value().second)
		{
		case Parameter::Horizontal:
			it->config.flip_mode.horizontal = false;
			break;
		case Parameter::Vertical:
			it->config.flip_mode.vertical = false;
			break;
		case Parameter::AdditiveColour:
			it->config.blend_mode = SDL_BLENDMODE_BLEND;
			break;
		}
	}
	void ParameterAction::update(const float& value_percent, const Parameter& from, const Parameter& to)
	{
		const auto it = target_object.lock();
		switch (to)
		{
		case Parameter::Horizontal:
			it->config.flip_mode.horizontal = true;
			break;
		case Parameter::Vertical:
			it->config.flip_mode.vertical = true;
			break;
		case Parameter::AdditiveColour:
			it->config.blend_mode = SDL_BLENDMODE_ADD;
			break;
		}
	}
	void ParameterAction::on_next_sequence() { reset_to_previous(); }
	void ParameterAction::on_finished() { reset_to_previous(); }
	void ParameterAction::on_started() { }
	ParameterAction::ParameterAction(std::weak_ptr<Structures::Render::Object::Object> target_object, const OsuParser::Beatmap::Objects::Event::Commands::ParameterCommand& osu_parameter)
		: ParameterAction(
			osu_parameter.startTime, osu_parameter.endTime, osu_parameter.easing, std::move(target_object),
			static_cast<Parameter>(osu_parameter.parameter), static_cast<Parameter>(osu_parameter.parameter),
			osu_parameter.sequence)
	{
	}
}