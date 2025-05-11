#include "structures/game/event.h" // Header
#include "structures/events/action/render.hpp"
#include "structures/events/condition/playing.h"
#include "structures/render/object/animation.h"

namespace fs = std::filesystem;

namespace Structures::Game::Beatmap
{
	namespace Event
	{
		static Structures::Events::Action::Buffer::CONTAINER load_commands(
			const std::weak_ptr<Render::Object::Object>& target_object,
			const Commands::Commands& commands,
			Structures::Events::Action::Buffer& action_buffer,
			Structures::Events::Event::Buffer* event_buffer)
		{
			using namespace Type::Commands;

			Structures::Events::Action::Buffer::CONTAINER actions;
			for (const auto& command : commands)
			{
				switch (command->type)
				{
				case EventCommandType::Fade:
				{
					if (const auto& fade = std::dynamic_pointer_cast<Commands::FadeCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::Render::FadeAction>(target_object, *fade);
						actions.emplace(fade->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::Move:
				{
					if (const auto& move = std::dynamic_pointer_cast<Commands::MoveCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::Render::MoveAction>(target_object, *move);
						actions.emplace(move->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::MoveX:
				{
					if (const auto& move = std::dynamic_pointer_cast<Commands::MoveXCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::Render::MoveXAction>(target_object, *move);
						actions.emplace(move->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::MoveY:
				{
					if (const auto& move = std::dynamic_pointer_cast<Commands::MoveYCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::Render::MoveYAction>(target_object, *move);
						actions.emplace(move->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::Scale:
				{
					if (const auto& scale = std::dynamic_pointer_cast<Commands::ScaleCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::Render::ScaleAction>(target_object, *scale);
						actions.emplace(scale->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::VectorScale:
				{
					if (const auto& scale = std::dynamic_pointer_cast<Commands::VectorScaleCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::Render::ScaleAction>(target_object, *scale);
						actions.emplace(scale->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::Rotate:
				{
					if (const auto& rotate = std::dynamic_pointer_cast<Commands::RotateCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::Render::RotateAction>(target_object, *rotate);
						actions.emplace(rotate->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::Color:
				{
					if (const auto& color = std::dynamic_pointer_cast<Commands::ColorCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::Render::ColorAction>(target_object, *color);
						actions.emplace(color->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::Parameter:
				{
					if (const auto& parameter = std::dynamic_pointer_cast<Commands::ParameterCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::Render::ParameterAction>(target_object, *parameter);
						actions.emplace(parameter->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::Loop:
				{
					if (const auto& loop = std::dynamic_pointer_cast<Commands::LoopCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::LoopAction>(&action_buffer.data, loop->startTime, loop->loopCount);
						action->add(load_commands(target_object, *loop->commands, action_buffer, event_buffer));
						actions.emplace(loop->startTime, std::move(action));
					}
				}
				break;

				case EventCommandType::Trigger:
				{
					if (const auto& trigger = std::dynamic_pointer_cast<Commands::TriggerCommand>(command))
					{
						auto action = std::make_shared<Structures::Events::Action::ConditionalAction>(
							&action_buffer.data, event_buffer, trigger->startTime, trigger->endTime,
							Structures::Events::Condition::Playing::make_condition(trigger->triggerType));
						action->add(load_commands(target_object, *trigger->commands, action_buffer, event_buffer));
						actions.emplace(trigger->startTime, std::move(action));
					}
				}
				break;
				}
			}
			return actions;
		}

		static Structures::Events::Action::Render::FadeAction made_show_command(std::weak_ptr<Render::Object::Object> target_object, const int64_t& time)
		{
			return { time, time, Structures::Events::Time::Easing::EasingFunctionType::Linear, std::move(target_object),
				Render::Object::SDL_MAX_ALPHA, Render::Object::SDL_MAX_ALPHA };
		}
		static Structures::Events::Action::Render::FadeAction made_hide_command(std::weak_ptr<Render::Object::Object> target_object, const int64_t& time)
		{
			return { time, time, Structures::Events::Time::Easing::EasingFunctionType::Linear, std::move(target_object), 0, 0 };
		}

		//! EventObjects
		std::shared_ptr<Render::Object::Collection>& EventObjects::pick_layer(const Type::Objects::Args::Layer::ImageLayer& layer)
		{
			switch (layer)
			{
			case Type::Objects::Args::Layer::ImageLayer::Background: return background;
			case Type::Objects::Args::Layer::ImageLayer::Fail: return fail;
			case Type::Objects::Args::Layer::ImageLayer::Pass: return pass;
			case Type::Objects::Args::Layer::ImageLayer::Foreground: return foreground;
			}
			return normal;
		}

		void EventObjects::submit_to_buffer(
			Render::Layer::Layer* normal,
			Render::Layer::Layer* background, Render::Layer::Layer* fail, Render::Layer::Layer* pass, Render::Layer::Layer* foreground) const
		{
			normal->render_buffer.add(this->normal);
			background->render_buffer.add(this->background);
			fail->render_buffer.add(this->fail);
			pass->render_buffer.add(this->pass);
			foreground->render_buffer.add(this->foreground);
		}
		void EventObjects::load_background_object(
			const fs::path& beatmap_root, SDL_Renderer* renderer,
			Structures::Events::Action::Buffer& action_buffer,
			const Objects::BackgroundObject& object)
		{
			using namespace Structures::Types::Render;

			const auto& file = beatmap_root / object.filename;

			//! Object
			const auto texture = memory.load(file, file.lexically_relative(beatmap_root).generic_string(), true);
			const SDL_FPoint pos = { static_cast<float>(object.x_offset), static_cast<float>(object.y_offset) };
			auto background = std::make_shared<Render::Object::Object>(texture, OriginType::TopLeft, pos);
			int window_w, window_h;
			SDL_GetRenderOutputSize(renderer, &window_w, &window_h);
			background->set_render_size({ static_cast<float>(window_w), static_cast<float>(window_h) });
			background->config.color.a = 0;
			normal->data.emplace_back(background);

			//! Action
			auto action = std::make_shared<Structures::Events::Action::Render::FadeAction>(
				object.startTime, object.startTime, EasingFunctionType::Linear, background,
				Render::Object::SDL_MAX_ALPHA, Render::Object::SDL_MAX_ALPHA);
			action_buffer.data.emplace(object.startTime, std::move(action));
		}
		void EventObjects::load_sprite_object(
			const fs::path& beatmap_root,
			Structures::Events::Action::Buffer& action_buffer,
			Structures::Events::Event::Buffer* event_buffer,
			const Objects::SpriteObject& object)
		{
			using namespace Structures::Types::Render;

			const auto& file = beatmap_root / object.filepath;
			const auto texture_name = file.lexically_relative(beatmap_root).generic_string();
			const auto& collection = pick_layer(object.layer);

			//! Object
			memory.load(file, texture_name, false);
			const auto texture = memory.find(texture_name);
			const SDL_FPoint pos = { static_cast<float>(object.x), static_cast<float>(object.y) };
			auto sprite = std::make_shared<Render::Object::Object>(texture, static_cast<OriginType>(object.origin), pos);
			collection->data.emplace_back(sprite);

			//! Action
			auto cmd = load_commands(sprite, *object.commands, action_buffer, event_buffer);
			int64_t start_time = (cmd.empty() ? 0 : cmd.begin()->first);
			int64_t end_time = (cmd.empty() ? 0 : cmd.begin()->second->end_time);
			for (auto& [time, action] : cmd)
			{
				start_time = std::min(start_time, time);
				end_time = std::max(end_time, action->end_time);
			}
			// Chỉ hiện khi object đến lúc
 			sprite->config.color.a = 0;
			if (!cmd.empty())
			{
				action_buffer.data.emplace(start_time,
					std::make_shared<Structures::Events::Action::Render::FadeAction>(
						made_show_command(sprite, start_time)));
			}
			// Phần Command
			for (auto& [time, action] : cmd)
				action_buffer.data.emplace(time, std::move(action));
			// Đến cuối luôn có một action để biến mất
			if (!cmd.empty())
				action_buffer.data.emplace(end_time,
					std::make_shared<Structures::Events::Action::Render::FadeAction>(
						made_hide_command(sprite, end_time)));
		}
		void EventObjects::load_animation_object(
			const fs::path& beatmap_root,
			Structures::Events::Action::Buffer& action_buffer,
			Structures::Events::Event::Buffer* event_buffer,
			Structures::Events::Time::Timer* timer,
			const Objects::AnimationObject& object)
		{
			using namespace Structures::Types::Render;

			const auto& file = beatmap_root / object.filepath;
			const auto& collection = pick_layer(object.layer);

			//! Object
			const auto parent = file.parent_path();
			// Trường hợp ".jpg" không hợp lệ trong đây nên phải check has extension
			const std::string stem = (file.has_extension()) ? file.stem().string() : "";
			const std::string extension = (file.has_extension()) ? file.extension().string() : file.stem().string();
			std::vector<Render::Texture::Memory::Item> frames;

			for (long long frame = 0; frame < object.frameCount; ++frame)
			{
				auto suffix = (object.frameCount >= 1 ? std::to_string(frame) : "") + extension;
				const auto frame_file = parent / (stem + suffix);
				const auto frame_name = frame_file.lexically_relative(beatmap_root).generic_string();
				memory.load(frame_file, frame_name, false);

				frames.emplace_back(memory.find(frame_name));
			}
			const SDL_FPoint pos = { static_cast<float>(object.x), static_cast<float>(object.y) };
			auto animation = std::make_shared<Render::Object::AnimationObject>(timer, std::move(frames), object.frameDelay,
				static_cast<LoopType>(object.looptype),
				static_cast<OriginType>(object.origin), pos);
			collection->data.emplace_back(animation);

			//! Action
			auto cmd = load_commands(animation, *object.commands, action_buffer, event_buffer);
			int64_t start_time = (cmd.empty() ? 0 : cmd.begin()->first);
			int64_t end_time = (cmd.empty() ? 0 : cmd.begin()->second->end_time);
			for (auto& [time, action] : cmd)
			{
				start_time = std::min(start_time, time);
				end_time = std::max(end_time, action->end_time);
			}
			// Chỉ hiện khi object đến lúc
			animation->config.color.a = 0;
			if (!cmd.empty())
			{
				action_buffer.data.emplace(start_time,
					std::make_shared<Structures::Events::Action::Render::FadeAction>(
						made_show_command(animation, start_time)));
			}
			// Phần Command
			for (auto& [time, action] : cmd)
				action_buffer.data.emplace(time, std::move(action));
			// Đến cuối luôn có một action để biến mất
			if (!cmd.empty())
				action_buffer.data.emplace(end_time,
					std::make_shared<Structures::Events::Action::Render::FadeAction>(
						made_hide_command(animation, end_time)));

		}
		EventObjects::EventObjects(
			const OsuParser::Beatmap::Objects::Event::Events& events,
			SDL_Renderer* renderer, const fs::path& beatmap_root,
			Structures::Events::Time::Timer* timer,
			Structures::Events::Action::Buffer& action_buffer,
			Structures::Events::Event::Buffer* event_buffer)
			: memory(renderer)
		{
			for (const auto& object : events.objects)
			{
				switch (object->type)
				{
				case Type::Objects::EventObjectType::Background:
					//load_background_object(beatmap_root, renderer, action_buffer, *std::dynamic_pointer_cast<Objects::BackgroundObject>(object));
					break;
				case Type::Objects::EventObjectType::Video:
					//TODO: Giải mã Video
					break;
				case Type::Objects::EventObjectType::Break:
					//TODO: Chuyển trạng thái
					break;
				case Type::Objects::EventObjectType::Sprite:
					load_sprite_object(beatmap_root, action_buffer, event_buffer, *std::dynamic_pointer_cast<Objects::SpriteObject>(object));
					break;
				case Type::Objects::EventObjectType::Animation:
					load_animation_object(beatmap_root, action_buffer, event_buffer, timer, *std::dynamic_pointer_cast<Objects::AnimationObject>(object));
					break;
				case Type::Objects::EventObjectType::Sample:
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