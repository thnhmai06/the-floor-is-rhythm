#pragma once
#include "config.h"
#include "engine/events/action/render.h"
#include "engine/render/object.h"
#include "structures/render/object/clickable.h"

namespace Structures::Screen::Pausing
{
	using namespace Engine::Render::Object;
	using Engine::Events::Action::Render::FadeAction;
	using Render::Object::MousePressObject;

	struct Pausing : Collection
	{
	protected:
		std::shared_ptr<Object> pause_background;
		std::shared_ptr<Object> fail_background;
		std::shared_ptr<MousePressObject> continue_button;
		std::shared_ptr<MousePressObject> retry_button;
		std::shared_ptr<MousePressObject> back_button;

		static FadeAction make_fade_in(Object& object, const int64_t& current_time);
		static FadeAction make_fade_out(Object& object, const int64_t& current_time);

	public:
		Engine::Events::Action::Buffer* action_buffer;
		std::function<void()> on_continue;
		std::function<void()> on_retry;
		std::function<void()> on_back;

		void pause(bool fail = false);
		void resume();
		void update() const;

		explicit Pausing(
			const Memory& skin,
			const Engine::Events::Event::External::Buffer& external_buffer,
			Engine::Events::Action::Buffer& action_buffer,
			std::function<void()> on_continue = nullptr,
			std::function<void()> on_retry = nullptr,
			std::function<void()> on_back = nullptr);
	};
}
