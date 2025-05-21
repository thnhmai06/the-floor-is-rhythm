#pragma once
#include <osu!parser/Parser/Structures/Beatmap/Objects/Event.hpp>
#include "engine/events/action.h"
#include "engine/render/layer.h"

namespace Structures::Game::Beatmap
{
	namespace Event
	{
		using namespace Engine::Render::Object;
		using ActionBuffer = Engine::Events::Action::Buffer;
		using InternalEventBuffer = Engine::Events::Event::Internal::Buffer;
		using Engine::Render::Texture::Memory;
		using Engine::Render::Layer::Layer;
		using namespace Engine::Events::Timing;

		using Events = OsuParser::Beatmap::Objects::Event::Events;

		//! Chú ý: Lưu trữ EventObjects này trong quá trình chơi
		//! để buffer có thể sử dụng các objects được
		struct EventObjects final
		{
		private:
			void load_background_object(
				const std::filesystem::path& beatmap_root,
				SDL_Renderer* renderer, ActionBuffer& action_buffer,
				const OsuParser::Beatmap::Objects::Event::Objects::BackgroundObject& object);
			void load_sprite_object(
				const std::filesystem::path& beatmap_root,
				ActionBuffer& action_buffer,
				InternalEventBuffer& event_buffer,
				const OsuParser::Beatmap::Objects::Event::Objects::SpriteObject& object);
			void load_animation_object(
				const std::filesystem::path& beatmap_root,
				ActionBuffer& action_buffer,
				InternalEventBuffer& event_buffer, const std::weak_ptr<Timer>& timer,
				const OsuParser::Beatmap::Objects::Event::Objects::AnimationObject& object);

		public:
			Memory memory;
			std::shared_ptr<Collection> normal = std::make_shared<Collection>();
			std::shared_ptr<Collection> background = std::make_shared<Collection>();
			std::shared_ptr<Collection> fail = std::make_shared<Collection>();
			std::shared_ptr<Collection> pass = std::make_shared<Collection>();
			std::shared_ptr<Collection> foreground = std::make_shared<Collection>();

			std::shared_ptr<Collection>& pick_layer(const OsuParser::Beatmap::Objects::Event::Type::Objects::Args::Layer::ImageLayer& layer);
			void submit_to_buffer(Buffer& background_buffer, Buffer& storyboard_buffer) const;

			explicit EventObjects(
				const OsuParser::Beatmap::Objects::Event::Events& events,
				SDL_Renderer* renderer, const std::filesystem::path& beatmap_root,
				const std::weak_ptr<Timer>& timer, ActionBuffer& action_buffer,
				InternalEventBuffer& event_buffer);
			~EventObjects();
		};
	}
}
