#pragma once
#include "config.h"
#include "structures/events/action/render.hpp"
#include "structures/render/layer.h"

namespace Structures::Screen::Pausing
{
	using namespace Render::Object;

	struct PauseScreen
	{
	protected:
		Render::Layer::Layer::Buffer::Item item;

		std::shared_ptr<Object> pause_background;
		std::shared_ptr<Object> fail_background;
		std::shared_ptr<ReactObject> continue_button;
		std::shared_ptr<ReactObject> retry_button;
		std::shared_ptr<ReactObject> back_button;
		std::shared_ptr<Collection> collection;

		bool incoming = false;
		bool is_paused = false;
		bool is_failed = false;

		static Events::Action::Render::FadeAction make_fade_in(std::weak_ptr<Object> object, const int64_t& current_time);
		static Events::Action::Render::FadeAction make_fade_out(std::weak_ptr<Object> object, const int64_t& current_time);

	public:
		std::function<void()> before_paused;
		std::function<void()> after_resume;
		std::function<void()> on_continue;
		std::function<void()> on_retry;
		std::function<void()> on_back;

		const bool& get_paused() const;
		const bool& get_failed() const;
		const bool& get_incoming() const;
		void make_fail();
		void make_pause();
		void make_unpause(const int64_t& function_delay = ::Config::Game::General::DELAY_RESUME);
		void toggle_pause(const int64_t& delay_after_resume = ::Config::Game::General::DELAY_RESUME);
		void check_event(const Events::Event::Input::MouseEvents& events) const;
		void clean(bool exit = false);

		explicit PauseScreen(
			const Memory& skin, Render::Layer::Layer* foreground,
			std::function<void()> before_paused = nullptr,
			std::function<void()> after_resume = nullptr,
			std::function<void()> on_continue = nullptr,
			std::function<void()> on_retry = nullptr,
			std::function<void()> on_back = nullptr);
		~PauseScreen();
	};
}
