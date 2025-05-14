#pragma once
#include "structures/events/action/action.h"
#include "structures/events/time/time.h"
#include "structures/render/layer.h"
#include "structures/render/object.h"
#include "structures/render/texture.h"

namespace Structures::Game::Beatmap
{
	namespace Event
	{
		using namespace OsuParser::Beatmap::Objects::Event;

		using Events = Event::Events;

		//! Chú ý: Lưu trữ EventObjects trong quá trình chơi
		//! để buffer có thể sử dụng các objects được
		struct EventObjects final
		{
		private:
			void load_background_object(
				const std::filesystem::path& beatmap_root,
				SDL_Renderer* renderer,
				Structures::Events::Action::Buffer& action_buffer,
				const Objects::BackgroundObject& object);
			void load_sprite_object(
				const std::filesystem::path& beatmap_root,
				Structures::Events::Action::Buffer& action_buffer,
				Structures::Events::Event::Buffer* event_buffer,
				const Objects::SpriteObject& object);
			void load_animation_object(
				const std::filesystem::path& beatmap_root,
				Structures::Events::Action::Buffer& action_buffer,
				Structures::Events::Event::Buffer* event_buffer,
				Structures::Events::Time::Timer* timer,
				const Objects::AnimationObject& object);

		public:
			Render::Texture::Memory memory;
			std::shared_ptr<Render::Object::Collection> normal = std::make_shared<Render::Object::Collection>();
			std::shared_ptr<Render::Object::Collection> background = std::make_shared<Render::Object::Collection>();
			std::shared_ptr<Render::Object::Collection> fail = std::make_shared<Render::Object::Collection>();
			std::shared_ptr<Render::Object::Collection> pass = std::make_shared<Render::Object::Collection>();
			std::shared_ptr<Render::Object::Collection> foreground = std::make_shared<Render::Object::Collection>();

			std::shared_ptr<Render::Object::Collection>& pick_layer(const Type::Objects::Args::Layer::ImageLayer& layer);
			void submit_to_buffer(Render::Layer::Layer::Buffer& background_buffer, Render::Layer::Layer::Buffer& storyboard_buffer) const;

			explicit EventObjects(
				const OsuParser::Beatmap::Objects::Event::Events& events,
				SDL_Renderer* renderer, const std::filesystem::path& beatmap_root,
				Structures::Events::Time::Timer* timer,
				Structures::Events::Action::Buffer& action_buffer,
				Structures::Events::Event::Buffer* event_buffer);
			~EventObjects();
		};
	}
}
