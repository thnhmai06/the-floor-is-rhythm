#include "structures/game/event.h" // Header
#include <ranges>
#include "engine/events/action/render.h"
#include "engine/render/object/animation.h"
#include "structures/events/condition/internal/gameplay.h"

namespace Structures::Game::Beatmap
{
	namespace Event
	{
		using namespace Engine::Events::Action::Render;

		static ActionBuffer::Container load_commands(
			Object& target_object,
			const OsuParser::Beatmap::Objects::Event::Commands::Commands& commands,
			ActionBuffer& action_buffer,
			InternalEventBuffer& event_buffer)
		{
			using namespace OsuParser::Beatmap::Objects::Event::Commands;
			using OsuParser::Beatmap::Objects::Event::Type::Commands::EventCommandType;
			using Utilities::Math::FPoint::to_float_point;

			ActionBuffer::Container actions;
			for (const auto& command : commands)
			{
				switch (command->type)
				{
				case EventCommandType::Fade:
				{
					if (const auto& fade = std::dynamic_pointer_cast<FadeCommand>(command))
					{
						auto action = std::make_shared<FadeAction>(
							fade->startTime, fade->endTime, static_cast<Easing::Easing>(fade->easing),
							target_object.config, fade->startOpacity, fade->endOpacity, fade->sequence);
						actions.emplace(action->start_time, std::move(action));
					}
				}
				break;

				case EventCommandType::Move:
				{
					if (const auto& move = std::dynamic_pointer_cast<MoveCommand>(command))
					{
						auto action = std::make_shared<MoveAction>(
							move->startTime, move->endTime, static_cast<Easing::Easing>(move->easing),
							target_object.config, to_float_point(move->startX, move->startY), to_float_point(move->endX, move->endY));
						for (const auto& [x, y] : move->sequence)
							action->sequence.push_back(to_float_point(x, y));
						actions.emplace(action->start_time, std::move(action));
					}
				}
				break;

				case EventCommandType::MoveX:
				{
					if (const auto& move_x = std::dynamic_pointer_cast<MoveXCommand>(command))
					{
						auto action = std::make_shared<MoveXAction>(
							move_x->startTime, move_x->endTime, static_cast<Easing::Easing>(move_x->easing),
							target_object.config, move_x->startX, move_x->endX);
						for (const auto& x : move_x->sequence)
							action->sequence.push_back(static_cast<float>(x));
						actions.emplace(action->start_time, std::move(action));
					}
				}
				break;

				case EventCommandType::MoveY:
				{
					if (const auto& move_y = std::dynamic_pointer_cast<MoveYCommand>(command))
					{
						auto action = std::make_shared<MoveYAction>(
							move_y->startTime, move_y->endTime, static_cast<Easing::Easing>(move_y->easing),
							target_object.config, move_y->startY, move_y->endY);
						for (const auto& y : move_y->sequence)
							action->sequence.push_back(static_cast<float>(y));
						actions.emplace(action->start_time, std::move(action));
					}
				}
				break;

				case EventCommandType::Scale:
				{
					if (const auto& scale = std::dynamic_pointer_cast<ScaleCommand>(command))
					{
						auto action = std::make_shared<ScaleAction>(
							scale->startTime, scale->endTime, static_cast<Easing::Easing>(scale->easing),
							target_object.config, 
							to_float_point(scale->startScale, scale->startScale), 
							to_float_point(scale->endScale, scale->endScale));
						for (const auto& s : scale->sequence)
							action->sequence.push_back(to_float_point(s, s));
						actions.emplace(action->start_time, std::move(action));
					}
				}
				break;

				case EventCommandType::VectorScale:
				{
					if (const auto& v_scale = std::dynamic_pointer_cast<VectorScaleCommand>(command))
					{
						auto action = std::make_shared<ScaleAction>(
							v_scale->startTime, v_scale->endTime, static_cast<Easing::Easing>(v_scale->easing), 
							target_object.config,
							to_float_point(v_scale->startXScale, v_scale->startYScale),
							to_float_point(v_scale->endXScale, v_scale->endYScale));
						for (const auto& [x, y] : v_scale->sequence)
							action->sequence.push_back(to_float_point(x, y));
						actions.emplace(action->start_time, std::move(action));
					}
				}
				break;

				case EventCommandType::Rotate:
				{
					using Utilities::Math::radian_to_degree;

					if (const auto& rotate = std::dynamic_pointer_cast<RotateCommand>(command))
					{
						auto action = std::make_shared<RotateAction>(
							rotate->startTime, rotate->endTime, static_cast<Easing::Easing>(rotate->easing), 
							target_object.config, 
							radian_to_degree(rotate->startRotate), radian_to_degree(rotate->endRotate));
						for (const auto& r : rotate->sequence)
							action->sequence.push_back(radian_to_degree(r));
						actions.emplace(action->start_time, std::move(action));
					}
				}
				break;

				case EventCommandType::Color:
				{
					if (const auto& color = std::dynamic_pointer_cast<ColorCommand>(command))
					{
						auto action = std::make_shared<ColorAction>(
							color->startTime, color->endTime, static_cast<Easing::Easing>(color->easing), target_object.config,
							Color { static_cast<uint8_t>(color->startR), static_cast<uint8_t>(color->startG), static_cast<uint8_t>(color->startB) }, 
							Color { static_cast<uint8_t>(color->endR), static_cast<uint8_t>(color->endG), static_cast<uint8_t>(color->endB) });
						for (const auto& [r, g, b] : color->sequence)
							action->sequence.emplace_back(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));
						actions.emplace(action->start_time, std::move(action));
					}
				}
				break;

				case EventCommandType::Parameter:
				{
					if (const auto& parameter = std::dynamic_pointer_cast<ParameterCommand>(command))
					{
						auto action = std::make_shared<ParameterAction>(
							parameter->startTime, parameter->endTime, static_cast<Easing::Easing>(parameter->easing), 
							target_object.config, static_cast<Parameter>(parameter->parameter));
						for (const auto& p : parameter->sequence)
							action->sequence.push_back(static_cast<Parameter>(p));
						actions.emplace(action->start_time, std::move(action));
					}
				}
				break;

				case EventCommandType::Loop:
				{
					if (const auto& loop = std::dynamic_pointer_cast<LoopCommand>(command))
					{
						auto action = std::make_shared<Engine::Events::Action::LoopAction>(&action_buffer.data, loop->startTime, loop->loopCount);
						action->add_callback(load_commands(target_object, *loop->commands, action_buffer, event_buffer));
						actions.emplace(loop->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::Trigger:
				{
					if (const auto& trigger = std::dynamic_pointer_cast<TriggerCommand>(command))
					{
						auto action = std::make_shared<Engine::Events::Action::ConditionalAction>(
							&action_buffer.data, event_buffer, trigger->startTime, trigger->endTime,
							Structures::Events::Condition::Internal::Gameplay::make_condition(trigger->triggerType));
						action->add(load_commands(target_object, *trigger->commands, action_buffer, event_buffer));
						actions.emplace(trigger->startTime, std::move(action));
					}
				}
				break;
				}
			}
			return actions;
		}

		static FadeAction made_show_command(Object& target_object, const Time& time)
		{
			return { time, time, Easing::Easing::Linear, target_object.config, 1.0, 1.0 };
		}
		static FadeAction made_hide_command(Object& target_object, const Time& time)
		{
			return { time, time, Easing::Easing::Linear, target_object.config, 0.0, 0.0 };
		}

		//! EventObjects
		std::shared_ptr<Collection>& EventObjects::pick_layer(const OsuParser::Beatmap::Objects::Event::Type::Objects::Args::Layer::ImageLayer& layer)
		{
			using OsuParser::Beatmap::Objects::Event::Type::Objects::Args::Layer::ImageLayer;

			switch (layer)
			{
			case ImageLayer::Background: return background;
			case ImageLayer::Fail: return fail;
			case ImageLayer::Pass: return pass;
			case ImageLayer::Foreground: return foreground;
			}
			return normal;
		}

		void EventObjects::submit_to_buffer(Buffer& background_buffer, Buffer& storyboard_buffer) const
		{
			background_buffer.add(normal);

			storyboard_buffer.add(background);
			storyboard_buffer.add(fail);
			storyboard_buffer.add(pass);
			storyboard_buffer.add(foreground);
		}
		void EventObjects::load_background_object(
			const std::filesystem::path& beatmap_root,
			SDL_Renderer* renderer, ActionBuffer& action_buffer,
			const OsuParser::Beatmap::Objects::Event::Objects::BackgroundObject& object)
		{
			using Engine::Render::OriginType;
			using Easing::Easing;
			const auto& file = beatmap_root / object.filename;

			//! Object
			const auto texture = memory.load(file, file.lexically_relative(beatmap_root).generic_string(), true);
			const SDL_FPoint pos = Utilities::Math::FPoint::to_float_point(object.x_offset, object.y_offset);
			auto background = std::make_shared<Object>(texture, OriginType::TopLeft, pos);
			int window_w, window_h;
			SDL_GetRenderOutputSize(renderer, &window_w, &window_h);
			background->set_render_size(Utilities::Math::FPoint::to_float_point(window_w, window_h));
			background->config.alpha.percent = 0;
			normal->data.emplace_back(background);

			//! Action
			auto action = std::make_shared<FadeAction>(object.startTime, object.startTime, Easing::Linear, background->config, 1.0, 1.0);
			action_buffer.data.emplace(object.startTime, std::move(action));
		}
		void EventObjects::load_sprite_object(
			const std::filesystem::path& beatmap_root,
			ActionBuffer& action_buffer, InternalEventBuffer& event_buffer,
			const OsuParser::Beatmap::Objects::Event::Objects::SpriteObject& object)
		{
			using Engine::Render::OriginType;
			const auto& file = beatmap_root / object.filepath;
			const auto texture_name = file.lexically_relative(beatmap_root).generic_string();
			const auto& collection = pick_layer(object.layer);

			//! Object
			memory.load(file, texture_name, false);
			const auto texture = memory.find(texture_name);
			const SDL_FPoint pos = Utilities::Math::FPoint::to_float_point(object.x, object.y);
			auto sprite = std::make_shared<Object>(texture, static_cast<OriginType>(object.origin), pos);
			collection->data.emplace_back(sprite);

			//! Action
			auto cmd = load_commands(*sprite, *object.commands, action_buffer, event_buffer);
			int64_t start_time = (cmd.empty() ? 0 : cmd.begin()->first);
			int64_t end_time = (cmd.empty() ? 0 : cmd.begin()->second->end_time);
			std::optional<float> start_pos_x, start_pos_y;
			std::optional<SDL_FPoint> start_scale;
			std::optional<float> start_rotate;
			std::optional<Color> start_color;
			bool start_horizontal = false, start_vertical = false, start_additive = false;
			for (auto& [time, action] : cmd)
			{
				start_time = std::min(start_time, time);
				end_time = std::max(end_time, action->end_time);
			}
			for (auto& action : cmd | std::views::values)
			{
				if (!start_pos_x.has_value())
				{
					if (const auto move = std::dynamic_pointer_cast<MoveAction>(action))
						start_pos_x = move->from.x;
					if (const auto move_x = std::dynamic_pointer_cast<MoveXAction>(action))
						start_pos_x = move_x->from;
				}
				if (!start_pos_y.has_value())
				{
					if (const auto move = std::dynamic_pointer_cast<MoveAction>(action))
						start_pos_y = move->from.y;
					if (const auto move_y = std::dynamic_pointer_cast<MoveYAction>(action))
						start_pos_y = move_y->from;
				}
				if (!start_scale.has_value())
				{
					if (const auto scale = std::dynamic_pointer_cast<ScaleAction>(action))
						start_scale = scale->from;
				}
				if (!start_rotate.has_value())
				{
					if (const auto rotate = std::dynamic_pointer_cast<RotateAction>(action))
						start_rotate = rotate->from;
				}
				if (!start_color.has_value())
				{
					if (const auto color = std::dynamic_pointer_cast<ColorAction>(action))
						start_color = color->from;
				}
				if (const auto parameter = std::dynamic_pointer_cast<ParameterAction>(action);
					parameter && parameter->start_time == start_time)
				{
					switch (parameter->to)
					{
					case Parameter::Horizontal:
						start_horizontal = true;
						break;
					case Parameter::Vertical:
						start_vertical = true;
						break;
					case Parameter::AdditiveColour:
						start_additive = true;
						break;
					}
				}
			}

			// Căn chỉnh đúng với trạng thái đầu tiên xuất hiện
			// ref: https://discord.com/channels/188630481301012481/1097318920991559880/1372195997043785789 - osu why?
			if (start_pos_x.has_value()) sprite->config.render_pos.x = start_pos_x.value();
			if (start_pos_y.has_value()) sprite->config.render_pos.y = start_pos_y.value();
			if (start_scale.has_value()) sprite->config.scale = start_scale.value();
			if (start_rotate.has_value()) sprite->config.angle = start_rotate.value();
			if (start_color.has_value()) sprite->config.color = start_color.value();
			if (start_horizontal) sprite->config.flip_mode.horizontal = true;
			if (start_vertical) sprite->config.flip_mode.vertical = true;
			if (start_additive) sprite->config.blend_mode = SDL_BLENDMODE_ADD;

			// Chỉ hiện khi object đến lúc
			sprite->config.alpha.percent = 0;
			if (!cmd.empty())
				action_buffer.data.emplace(start_time, std::make_shared<FadeAction>(made_show_command(*sprite, start_time)));
			// Phần Command
			for (auto& [time, action] : cmd)
				action_buffer.data.emplace(time, std::move(action));
			// Đến cuối luôn có một action để biến mất
			if (!cmd.empty())
				action_buffer.data.emplace(end_time, std::make_shared<FadeAction>(made_hide_command(*sprite, end_time)));
		}
		void EventObjects::load_animation_object(
			const std::filesystem::path& beatmap_root,
			ActionBuffer& action_buffer,
			InternalEventBuffer& event_buffer, const std::weak_ptr<Timer>& timer,
			const OsuParser::Beatmap::Objects::Event::Objects::AnimationObject& object)
		{
			using Engine::Render::OriginType;
			const auto& file = beatmap_root / object.filepath;
			const auto& collection = pick_layer(object.layer);

			//! Object
			const auto parent = file.parent_path();
			// Trường hợp file chỉ mỗi ".jpg" không hợp lệ trong đây nên phải check has extension
			const std::string stem = (file.has_extension()) ? file.stem().string() : "";
			const std::string extension = (file.has_extension()) ? file.extension().string() : file.stem().string();
			std::vector<Memory::Item> frames;

			for (long long frame = 0; frame < object.frameCount; ++frame)
			{
				auto suffix = (object.frameCount >= 1 ? std::to_string(frame) : "") + extension;
				const auto frame_file = parent / (stem + suffix);
				const auto frame_name = frame_file.lexically_relative(beatmap_root).generic_string();
				memory.load(frame_file, frame_name, false);

				frames.emplace_back(memory.find(frame_name));
			}
			const SDL_FPoint pos = Utilities::Math::FPoint::to_float_point(object.x, object.y);
			auto animation = std::make_shared<AnimationObject>(
				timer, std::move(frames), object.frameDelay,
				static_cast<LoopType>(object.looptype),
				static_cast<OriginType>(object.origin), pos);
			collection->data.emplace_back(animation);

			//! Action
			auto cmd = load_commands(*animation, *object.commands, action_buffer, event_buffer);
			int64_t start_time = (cmd.empty() ? 0 : cmd.begin()->first);
			int64_t end_time = (cmd.empty() ? 0 : cmd.begin()->second->end_time);
			std::optional<float> start_pos_x, start_pos_y;
			std::optional<SDL_FPoint> start_scale;
			std::optional<float> start_rotate;
			std::optional<Color> start_color;
			bool start_horizontal = false, start_vertical = false, start_additive = false;
			for (auto& [time, action] : cmd)
			{
				start_time = std::min(start_time, time);
				end_time = std::max(end_time, action->end_time);
			}
			// Căn chỉnh đúng với trạng thái đầu tiên xuất hiện
			// ref: https://discord.com/channels/188630481301012481/1097318920991559880/1372195997043785789 - osu why?
			for (auto& action : cmd | std::views::values)
			{
				if (!start_pos_x.has_value())
				{
					if (const auto move = std::dynamic_pointer_cast<MoveAction>(action))
						start_pos_x = move->from.x;
					if (const auto move_x = std::dynamic_pointer_cast<MoveXAction>(action))
						start_pos_x = move_x->from;
				}
				if (!start_pos_y.has_value())
				{
					if (const auto move = std::dynamic_pointer_cast<MoveAction>(action))
						start_pos_y = move->from.y;
					if (const auto move_y = std::dynamic_pointer_cast<MoveYAction>(action))
						start_pos_y = move_y->from;
				}
				if (!start_scale.has_value())
				{
					if (const auto scale = std::dynamic_pointer_cast<ScaleAction>(action))
						start_scale = scale->from;
				}
				if (!start_rotate.has_value())
				{
					if (const auto rotate = std::dynamic_pointer_cast<RotateAction>(action))
						start_rotate = rotate->from;
				}
				if (!start_color.has_value())
				{
					if (const auto color = std::dynamic_pointer_cast<ColorAction>(action))
						start_color = color->from;
				}
				if (const auto parameter = std::dynamic_pointer_cast<ParameterAction>(action);
					parameter && parameter->start_time == start_time)
				{
					switch (parameter->to)
					{
					case Parameter::Horizontal:
						start_horizontal = true;
						break;
					case Parameter::Vertical:
						start_vertical = true;
						break;
					case Parameter::AdditiveColour:
						start_additive = true;
						break;
					}
				}
			}
			if (start_pos_x.has_value()) animation->config.render_pos.x = start_pos_x.value();
			if (start_pos_y.has_value()) animation->config.render_pos.y = start_pos_y.value();
			if (start_scale.has_value()) animation->config.scale = start_scale.value();
			if (start_rotate.has_value()) animation->config.angle = start_rotate.value();
			if (start_color.has_value()) animation->config.color = start_color.value();
			if (start_horizontal) animation->config.flip_mode.horizontal = true;
			if (start_vertical) animation->config.flip_mode.vertical = true;
			if (start_additive) animation->config.blend_mode = SDL_BLENDMODE_ADD;

			// Chỉ hiện khi object đến lúc
			animation->config.alpha.percent = 0;
			if (!cmd.empty())
				action_buffer.data.emplace(start_time, std::make_shared<FadeAction>(made_show_command(*animation, start_time)));
			// Phần Command
			for (auto& [time, action] : cmd)
				action_buffer.data.emplace(time, std::move(action));
			// Đến cuối luôn có một action để biến mất
			if (!cmd.empty())
				action_buffer.data.emplace(end_time, std::make_shared<FadeAction>(made_hide_command(*animation, end_time)));
		}
		EventObjects::EventObjects(
			const OsuParser::Beatmap::Objects::Event::Events& events,
			SDL_Renderer* renderer, const std::filesystem::path& beatmap_root,
			const std::weak_ptr<Timer>& timer, ActionBuffer& action_buffer,
			InternalEventBuffer& event_buffer)
			: memory(renderer)
		{
			using OsuParser::Beatmap::Objects::Event::Type::Objects::EventObjectType;
			namespace OsuEventObject = OsuParser::Beatmap::Objects::Event::Objects;
			for (const auto& object : events.objects)
			{
				switch (object->type)
				{
				case EventObjectType::Background:
					//load_background_object(beatmap_root, renderer, action_buffer, *std::dynamic_pointer_cast<Objects::BackgroundObject>(object));
					break;
				case EventObjectType::Video:
					//TODO: Giải mã Video
					break;
				case EventObjectType::Break:
					//TODO: Chuyển trạng thái
					break;
				case EventObjectType::Sprite:
					load_sprite_object(beatmap_root, action_buffer, event_buffer, 
						*std::dynamic_pointer_cast<OsuEventObject::SpriteObject>(object));
					break;
				case EventObjectType::Animation:
					load_animation_object(beatmap_root, action_buffer, event_buffer, timer, 
						*std::dynamic_pointer_cast<OsuEventObject::AnimationObject>(object));
					break;
				case EventObjectType::Sample:
					//TODO: Chưa hỗ trợ :(
					break;
				}
			}
		}
		EventObjects::~EventObjects()
		{
			normal.reset();
			background.reset();
			fail.reset();
			pass.reset();
			foreground.reset();
		}
	}
}