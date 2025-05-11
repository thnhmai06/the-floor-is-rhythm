#include "structures/screen/pausing.h" // Header
#include "core/resources/event.h"
#include "format/skin.h"
#include "config.h"

namespace Structures::Screen::Pausing
{
	using namespace Format::Skin::Image::Pausing;
	using namespace ::Config::Game::Render::Pausing;
	using namespace Core::Resources::Event;

	Events::Action::Render::FadeAction PauseScreen::make_fade_in(std::weak_ptr<Object> object, const int64_t& current_time)
	{
		return{ current_time, current_time + TIME_FADE_IN, Types::Render::EasingFunctionType::CircOut, std::move(object), 0, SDL_MAX_ALPHA };
	}
	Events::Action::Render::FadeAction PauseScreen::make_fade_out(std::weak_ptr<Object> object, const int64_t& current_time)
	{
		return{ current_time, current_time + TIME_FADE_OUT, Types::Render::EasingFunctionType::CircOut, std::move(object), SDL_MAX_ALPHA, 0 };
	}
	const bool& PauseScreen::get_paused() const
	{
		return is_paused;
	}
	const bool& PauseScreen::get_failed() const
	{
		return is_failed;
	}

	const bool& PauseScreen::get_incoming() const
	{
		return incoming;
	}
	void PauseScreen::make_pause()
	{
		if (incoming || is_failed) return;

		const auto current_time = runtime.get_time();
		incoming = true;
		before_paused();
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_in(pause_background, current_time)));
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_in(continue_button, current_time)));
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_in(retry_button, current_time)));
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_in(back_button, current_time)));
		action_buffer.data.emplace(current_time + TIME_FADE_IN,
			std::make_shared<Events::Action::CallbackAction>(
				current_time + TIME_FADE_IN, [&](const int64_t&) { is_paused = true; incoming = false; }));
	}
	void PauseScreen::make_unpause(const int64_t& function_delay)
	{
		if (incoming || is_failed) return;

		const auto current_time = runtime.get_time();
		incoming = true;
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_out(pause_background, current_time)));
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_out(continue_button, current_time)));
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_out(retry_button, current_time)));
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_out(back_button, current_time)));
		action_buffer.data.emplace(current_time + TIME_FADE_OUT + function_delay,
			std::make_shared<Events::Action::CallbackAction>(
				current_time + TIME_FADE_OUT + function_delay, [&](const int64_t&)
				{
					is_paused = false;
					incoming = false;
					after_resume();
				}));
	}

	void PauseScreen::make_fail()
	{
		const auto current_time = runtime.get_time();
		incoming = true;
		before_paused();
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_in(fail_background, current_time)));
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_in(retry_button, current_time)));
		action_buffer.data.emplace(current_time, std::make_shared<Events::Action::Render::FadeAction>(make_fade_in(back_button, current_time)));
		action_buffer.data.emplace(current_time,
			std::make_shared<Events::Action::CallbackAction>(
				current_time + TIME_FADE_IN, [&](const int64_t&) { is_failed = true; incoming = false; }));
	}

	void PauseScreen::toggle_pause(const int64_t& delay_after_resume)
	{
		if (!is_paused) make_pause();
		else make_unpause(delay_after_resume);
	}

	void PauseScreen::check_event(const Events::Event::Input::MouseEvents& events) const
	{
		if (incoming) return;

		continue_button->check_event(events);
		retry_button->check_event(events);
		back_button->check_event(events);
	}

	void PauseScreen::clean(const bool exit)
	{
		incoming = is_failed = is_paused = false;
		pause_background->config.color.a = 0;
		fail_background->config.color.a = 0;
		continue_button->config.color.a = 0;
		retry_button->config.color.a = 0;
		back_button->config.color.a = 0;

		if (exit)
		{
			item.destroy();

			pause_background.reset();
			fail_background.reset();
			continue_button.reset();
			retry_button.reset();
			back_button.reset();
			collection.reset();
		}
	}

	PauseScreen::PauseScreen(
		const Memory& skin, Render::Layer::Layer* foreground,
		std::function<void()> before_paused,
		std::function<void()> after_resume,
		std::function<void()> on_continue,
		std::function<void()> on_retry,
		std::function<void()> on_back)
		: before_paused(std::move(before_paused)), after_resume(std::move(after_resume)),
		on_continue(std::move(on_continue)), on_retry(std::move(on_retry)), on_back(std::move(on_back))
	{
		pause_background = std::make_shared<Object>(skin.find(Pausing::pause_background), Types::Render::OriginType::TopLeft);
		pause_background->set_render_size(Utilities::Math::FPoint::to_float_point(::Config::user_config->graphic.window_size));

		fail_background = std::make_shared<Object>(skin.find(Pausing::fail_background), Types::Render::OriginType::TopLeft);
		fail_background->set_render_size(Utilities::Math::FPoint::to_float_point(::Config::user_config->graphic.window_size));

		continue_button = std::make_shared<ReactObject>(skin.find(pause_continue), ORIGIN_CONTINUE, get_continue_pos(),
			[&](const SDL_MouseButtonEvent& event) {if (event.down) this->on_continue(); });
		continue_button->set_render_size(get_button_size());

		retry_button = std::make_shared<ReactObject>(skin.find(pause_retry), ORIGIN_RETRY, get_retry_pos(),
			[&](const SDL_MouseButtonEvent& event) {if (event.down) this->on_retry(); });
		retry_button->set_render_size(get_button_size());

		back_button = std::make_shared<ReactObject>(skin.find(pause_back), ORIGIN_BACK, get_back_pos(),
			[&](const SDL_MouseButtonEvent& event) {if (event.down) this->on_back(); });
		back_button->set_render_size(get_button_size());

		collection = std::make_shared<Collection>();
		collection->data.emplace_back(pause_background);
		collection->data.emplace_back(fail_background);
		collection->data.emplace_back(continue_button);
		collection->data.emplace_back(retry_button);
		collection->data.emplace_back(back_button);

		item = foreground->render_buffer.add(collection);
	}

	PauseScreen::~PauseScreen()
	{
		clean(true);
	}
}