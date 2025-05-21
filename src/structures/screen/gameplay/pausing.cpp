#include "structures/screen/gameplay/pausing.h" // Header
#include "format/skin.h"
#include "config.h"

namespace Structures::Screen::Pausing
{
	using namespace Format::Skin::Image::Pausing;
	using namespace ::Config::Game::Render::Pausing;
	using Engine::Events::Timing::Easing::Easing;
	using Engine::Events::Action::CallbackAction;

	FadeAction Pausing::make_fade_in(Object& object, const int64_t& current_time)
	{
		return{ current_time, current_time + TIME_FADE_IN, Easing::CircOut, object.config, 0, 1 };
	}
	FadeAction Pausing::make_fade_out(Object& object, const int64_t& current_time)
	{
		return{ current_time, current_time + TIME_FADE_OUT, Easing::CircOut, object.config, 1, 0 };
	}
	void Pausing::pause(const bool fail)
	{
		auto timer = action_buffer->timer.lock();
		const auto current_time = SDL_GetTicks();

		if (fail)
		{
			continue_button->config.alpha.percent = 0;
			pause_background->config.alpha.percent = 0;

			fail_background->config.alpha.percent = 1;
		} else
		{
			continue_button->config.alpha.percent = 1;
			pause_background->config.alpha.percent = 1;

			fail_background->config.alpha.percent = 0;
		}
		action_buffer->submit(std::make_shared<FadeAction>(current_time, current_time + TIME_FADE_IN, Easing::CircOut, config, 0, 1));
	}
	void Pausing::resume()
	{
		const auto current_time = SDL_GetTicks();
		action_buffer->submit(std::make_shared<FadeAction>(current_time, current_time + TIME_FADE_IN, Easing::CircOut, config, 1, 0));
	}

	void Pausing::update() const
	{
		continue_button->update();
		retry_button->update();
		back_button->update();
	}

	Pausing::Pausing(
		const Memory& skin,
		const Engine::Events::Event::External::Buffer& external_buffer,
		Engine::Events::Action::Buffer& action_buffer,
		std::function<void()> on_continue,
		std::function<void()> on_retry,
		std::function<void()> on_back)
		: action_buffer(&action_buffer),
		on_continue(std::move(on_continue)), on_retry(std::move(on_retry)), on_back(std::move(on_back))
	{
		config.alpha.percent = 0;

		pause_background = std::make_shared<Object>(skin.find(Format::Skin::Image::Pausing::pause_background), Engine::Render::OriginType::TopLeft);
		pause_background->set_render_size(Utilities::Math::FPoint::to_float_point(::Config::user_config->graphic.window_size));
		data.emplace_back(pause_background);

		fail_background = std::make_shared<Object>(skin.find(Format::Skin::Image::Pausing::fail_background), Engine::Render::OriginType::TopLeft);
		fail_background->set_render_size(Utilities::Math::FPoint::to_float_point(::Config::user_config->graphic.window_size));
		data.emplace_back(fail_background);

		continue_button = std::make_shared<MousePressObject>(skin.find(pause_continue), external_buffer, ORIGIN_CONTINUE, get_continue_pos(),
			[&](Object*, const Render::Object::ExternalEvent& event) {if (event.button.down) this->on_continue(); });
		continue_button->set_render_size(get_button_size());
		data.emplace_back(continue_button);

		retry_button = std::make_shared<MousePressObject>(skin.find(pause_retry), external_buffer, ORIGIN_RETRY, get_retry_pos(),
			[&](Object*, const Render::Object::ExternalEvent& event) {if (event.button.down) this->on_retry(); });
		retry_button->set_render_size(get_button_size());
		data.emplace_back(retry_button);

		back_button = std::make_shared<MousePressObject>(skin.find(pause_back), external_buffer, ORIGIN_BACK, get_back_pos(),
			[&](Object*, const Render::Object::ExternalEvent& event) {if (event.button.down) this->on_back(); });
		back_button->set_render_size(get_button_size());
		data.emplace_back(back_button);
	}
}