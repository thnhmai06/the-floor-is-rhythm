// ReSharper disable CppClangTidyClangDiagnosticShadow
#include "structures/screens/gameplay/playing.h" // Header
#include <ranges>
#include <spdlog/spdlog.h>
#include "structures/screens/gameplay/playing/mapset.h"
#include "structures/screens/gameplay/playing/cursor.h"
#include "structures/screens/gameplay/playing/health_bar.h"
#include "structures/screens/gameplay/playing/score.h"
#include "work/render/layers.h"
#include "work/render/textures.h"
#include "utilities.hpp"
#include "work/audio.h"

//! PLayingScreen
namespace Structures::Screens::Gameplay
{
	// ::Core::Score::counter
	void PlayingScreen::Core::Score::ScoreCounter::update_accuracy()
	{
		if (count_300 + count_100 + count_50 + count_slider_tick + count_miss == 0) // tránh chia 0 
		{
			accuracy = 1; // 100%
			return;
		}
		// chia 10 để giảm số cần nhớ
		constexpr uint8_t SCORE_PERFECT = 30; // 300
		constexpr uint8_t SCORE_GREAT = 10; // 100
		constexpr uint8_t SCORE_BAD = 5; // 50
		constexpr uint8_t SCORE_SLIDER_TICK = 1; // 10

		const unsigned long total = count_300 * SCORE_PERFECT + count_100 * SCORE_GREAT + count_50 * SCORE_BAD + count_slider_tick * SCORE_SLIDER_TICK;
		const unsigned long total_if_perfect = (count_300 + count_100 + count_50 + count_miss) * SCORE_PERFECT + count_slider_tick * SCORE_SLIDER_TICK;
		accuracy = static_cast<float>(total) / static_cast<float>(total_if_perfect);
	}

	void PlayingScreen::Core::Score::ScoreCounter::reset()
	{
		count_300 = count_100 = count_50 = count_slider_tick = count_miss = 0;
	}

	unsigned long PlayingScreen::Core::Score::ScoreCounter::get_elapsed_objects_num() const
	{
		return count_300 + count_100 + count_50 + count_miss;
	}
	void PlayingScreen::Core::Score::ScoreCounter::add_count(const uint16_t& score, const unsigned long& num)
	{
		switch (score)
		{
		case 300: count_300 += num; break;
		case 100: count_100 += num; break;
		case 50: count_50 += num; break;
		case 10: count_slider_tick += num; break;
		default: count_miss += num; break;
		}
	}

	// ::Core::Score::SliderScore
	PlayingScreen::Core::Score::SliderScore::SliderScore(const std::shared_ptr<const Game::Beatmap::HitObjects::Slider::ActionInfo>& slider_action)
		: action_info(slider_action)
	{
	}

	// ::Core::Score
	void PlayingScreen::Core::Score::update_score()
	{
		counter.update_accuracy();

		// Score = ((700000 * max_combo / total_combo) + (300000 * accuracy ^ 10 * elapsed_objects / total_objects)) * mod_multiplier
		constexpr float BASE_COMBO = 700000;
		constexpr float BASE_ACCURACY = 300000;

		const float& accuracy = *counter.get_accuracy();
		const float elapsed_objects = static_cast<float>(counter.get_elapsed_objects_num());

		const float combo_score = BASE_COMBO * static_cast<float>(max_combo) / static_cast<float>(beatmap->stats.count.total_combo);
		const float accuracy_score = BASE_ACCURACY * std::powf(accuracy, 10) * elapsed_objects / static_cast<float>(beatmap->stats.get_total_objects_num());
		const float score_no_multiplier = (combo_score + accuracy_score) * mod_multiplier;
		this->score = score_no_multiplier * mod_multiplier;
	}
	float PlayingScreen::Core::Score::check_and_add_floor_score(const int64_t& current_time, const float& input_latency)
	{
		const auto previous_score = score;
		const float click_moment = static_cast<float>(current_time) - input_latency;
		auto click_num = key_stoke->click.get_recently_clicked_num();

		while (!floor->empty() && static_cast<float>(floor->front()->time) <= click_moment + beatmap->calculated_difficulty.od.miss)
		{
			const auto current_floor = floor->front();
			const int16_t floor_score = beatmap->calculated_difficulty.od.get_score(
				click_num > 0, click_moment,
				current_floor->time, current_floor->direction,
				key_stoke->direction.current_direction);
			if (floor_score < 0) break; // chưa đến
			if (click_num > 0) --click_num;

			floor->pop();
			counter.add_count(floor_score);
			if (floor_score == 0) current_combo = 0; else current_combo++;
			max_combo = std::max(current_combo, max_combo);
		}
		update_score();
		return score - previous_score;
	}
	float PlayingScreen::Core::Score::check_and_add_slider_score(const int64_t& current_time, const float& input_latency)
	{
		const float previous_score = score;
		const float hold_moment = static_cast<float>(current_time) - input_latency;

		if (!slider->empty() && static_cast<float>(slider->front()->time) <= hold_moment)
		{
			const auto& current_slider = slider->front();
			//! Ở đây đã vô hiệu hóa nhiều slider cùng lúc
			if (static_cast<float>(current_slider->end_time) < hold_moment) // đã xong slider
				slider->pop();
			else if (!active_sliders.contains(current_slider))
				active_sliders.insert({ current_slider, SliderScore{current_slider} });
		}

		auto slider = active_sliders.begin();
		while (slider != active_sliders.end())
		{
			// slider_time: time khi chọn mốc thời gian lúc bắt đầu slider
			const auto& action = slider->second.action_info.lock();
			const float current_slider_time = hold_moment - static_cast<float>(action->time);
			const float end_slider_time = static_cast<float>(action->end_time - action->time);

			//! Điểm tick
			if (current_slider_time < 0) return 0;
			if (slider->second.last_uncompleted_tick_slider_time <= current_slider_time
				&& slider->second.last_uncompleted_tick_slider_time <= end_slider_time)
			{
				if (Utilities::Math::is_equal(current_slider_time, slider->second.last_uncompleted_tick_slider_time, 5.0f)) // đã đến lúc hold tick (5ms)
				{
					if (key_stoke->click.is_hold())
					{
						slider->second.last_uncompleted_tick_slider_time += action->tick_length;
						++slider->second.completed_ticks_num;
						counter.add_count(10);
						current_combo++;
					}
				}
				else
				{
					current_combo = 0;
					slider->second.last_uncompleted_tick_slider_time += action->tick_length;
				}
			}

			//! Điểm slider bonus
			int16_t bonus_score = -1; // TODO: Cập nhật điểm note bắt đầu lấy dữ liệu ở đây
			if (current_slider_time >= end_slider_time)
			{
				if (slider->second.completed_ticks_num == 0) bonus_score = 0;
				else if (slider->second.completed_ticks_num >= action->tick_num) bonus_score = 300;
				else if (slider->second.completed_ticks_num >= static_cast<unsigned long>(ceil(static_cast<float>(action->tick_num) / 2)))
					bonus_score = 100;
				else bonus_score = 50;
				counter.add_count(bonus_score);
				slider = active_sliders.erase(slider);
			}
			else ++slider;
		}
		update_score();
		return score - previous_score;
	}
	const float* PlayingScreen::Core::Score::get_score() const { return &score; }
	const unsigned long* PlayingScreen::Core::Score::get_current_combo() const { return &current_combo; }
	const unsigned long* PlayingScreen::Core::Score::get_max_combo() const { return &max_combo; }
	const float* PlayingScreen::Core::Score::get_mod_multiplier() const { return &mod_multiplier; }

	PlayingScreen::Core::Score::Score(Core* core, const float& mod_multiplier)
		: key_stoke(&core->key_stoke), beatmap(core->playing_screen->beatmap.get()),
		floor(&core->floor), slider(&core->slider), mod_multiplier(mod_multiplier)
	{
	}

	// ::Core::Keystroke::KeyCounter
	void PlayingScreen::Core::Keystroke::KeyCounter::update(const KeyboardEventList& events)
	{
		recently_pressed_num = 0;
		for (const auto& event : events)
		{
			if (event.scancode != target) continue;
			is_hold = event.repeat; // nếu thả ra, sẽ có event KeyUp để cập nhật
			if (event.down && !event.repeat)
			{
				recently_pressed_num++;
				count++;
			}
			else recently_pressed_num = 0; // tránh trường hợp spam press và hold
		}
	}
	void PlayingScreen::Core::Keystroke::KeyCounter::reset()
	{
		count = recently_pressed_num = 0;
		is_hold = false;
	}
	PlayingScreen::Core::Keystroke::KeyCounter::KeyCounter(const SDL_Scancode& target) : target(target)
	{
	}

	// ::Core::Keystroke::Direction
	void PlayingScreen::Core::Keystroke::Direction::update(const KeyboardEventList& events)
	{
		// Cập nhật lần cuối cùng thay đổi direction trong beatmap_events
		for (auto event = events.rbegin(); event != events.rend(); ++event)
		{
			if (event->scancode == right.target)
				current_direction = Types::Game::Direction::Direction::RIGHT;
			else if (event->scancode == left.target)
				current_direction = Types::Game::Direction::Direction::LEFT;
			else if (event->scancode == up.target)
				current_direction = Types::Game::Direction::Direction::UP;
			else if (event->scancode == down.target)
				current_direction = Types::Game::Direction::Direction::DOWN;
		}

		right.update(events);
		left.update(events);
		up.update(events);
		down.update(events);
	}
	void PlayingScreen::Core::Keystroke::Direction::reset()
	{
		right.reset();
		left.reset();
		up.reset();
		down.reset();
	}

	// ::Core::Keystroke::Click
	void PlayingScreen::Core::Keystroke::Click::reset()
	{
		k1.reset();
		k2.reset();
	}
	uint16_t PlayingScreen::Core::Keystroke::Click::get_recently_clicked_num() const
	{
		return k1.recently_pressed_num + k2.recently_pressed_num;
	}
	bool PlayingScreen::Core::Keystroke::Click::is_hold() const
	{
		return k1.is_hold || k2.is_hold;
	}
	void PlayingScreen::Core::Keystroke::Click::update(const KeyboardEventList& events)
	{
		k1.update(events);
		k2.update(events);
	}

	// ::Core::Keystroke
	void PlayingScreen::Core::Keystroke::reset()
	{
		direction.reset();
		click.reset();
	}
	void PlayingScreen::Core::Keystroke::update(const KeyboardEventList& events)
	{
		direction.update(events);
		click.update(events);
	}

	// ::Core::Health
	bool PlayingScreen::Core::Health::update(const int16_t& note_score, const unsigned long& current_combo)
	{
		if (is_pause) return true;
		value = std::clamp(value + diff_hp->get_note_hp(note_score, current_combo), 0.0f, 1.0f);
		return no_fail || !(value <= 0 && note_score == 0);
	}
	PlayingScreen::Core::Health::Health(
		const Game::Beatmap::Metadata::CalculatedDifficulty::HealthPoint* diff_hp,
		const bool no_fail) : diff_hp(diff_hp), no_fail(no_fail)
	{
	}
	PlayingScreen::Core::Health::Health(const Game::Beatmap::Beatmap* beatmap, const bool no_fail) : Health(&beatmap->calculated_difficulty.hp, no_fail)
	{
	}

	// ::Core
	bool PlayingScreen::Core::is_paused() const { return timer.is_paused(); }
	void PlayingScreen::Core::pause()
	{
		timer.pause();
		health.is_pause = true;
	}
	void PlayingScreen::Core::resume()
	{
		timer.resume();
		health.is_pause = false;
	}
	void PlayingScreen::Core::make_time_step(const EventList& events, const float& input_latency)
	{
		//! Nhan Event
		KeyboardEventList keyboard_events{};
		for (const auto& event : events)
		{
			if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
				keyboard_events.push_back(event.key);
		}

		const auto current_time = timer.get_time();
		key_stoke.update(keyboard_events);

		//! Tính điểm
		score.check_and_add_floor_score(current_time, input_latency);
		score.check_and_add_slider_score(current_time, input_latency);

		//! Cập nhật current object
		// update timing point
		while (!inherited.empty() && current_time >= inherited.front()->get_time())
			current_timing_velocity = Utilities::Container::get_front_and_pop(inherited)->get_velocity();
		while (!uninherited.empty() && current_time >= uninherited.front()->get_time())
			current_beat_length = Utilities::Container::get_front_and_pop(uninherited)->beat_length;
		// what should i do...

		//! Cập nhật hướng đi & di chuyển
		const auto time_delta = static_cast<float>(current_time - previous_time);
		const auto distance = time_delta * playing_screen->beatmap->calculated_difficulty.velocity.speed * current_timing_velocity;

		const Types::Game::Direction::Direction current_direction =
			Utilities::Container::get_front_dual_queue(floor, slider,
				[](
					const std::weak_ptr<const Game::Beatmap::HitObjects::Floor::ActionInfo>& left, 
					const std::weak_ptr<const Game::Beatmap::HitObjects::Slider::ActionInfo>& right)
				{
					return right.lock()->time < left.lock()->time;
				}) ? slider.front()->direction : floor.front()->direction;
		//SPDLOG_INFO("Current Direction: {}", static_cast<uint8_t>(current_direction));
		const auto mapset = dynamic_cast<Playing::Mapset::Mapset*>(playing_screen->render.components.map.collection->get());
		mapset->move(distance, current_direction);

		// cho nay da moving duoc, nhung van de la
		// render khong duoc dung
		// so may be check render

		// TODO: make it relative
		const auto left = floor.size() + slider.size();
		const auto total = playing_screen->beatmap->stats.get_total_objects_num();
		const int64_t current_hit_object_num = total - left;
		mapset->render_range.front() = { current_hit_object_num - 5, current_hit_object_num + 5 };

		previous_time = current_time;
	}
	PlayingScreen::Core::Core(
		const PlayingScreen* playing_screen,
		const int64_t& start_time,
		const float& mod_multiplier,
		const bool no_fail)
		: playing_screen(playing_screen), timer(start_time), score(this, mod_multiplier), health(playing_screen->beatmap.get(), no_fail)
	{
		const auto& beatmap = playing_screen->beatmap;

		auto [floor, slider] = beatmap->make_action_queue();
		auto [inherited, uninherited] = playing_screen->beatmap->timing_points.split_to_queue();
		this->floor = std::move(floor);
		this->slider = std::move(slider);
		this->inherited = std::move(inherited);
		this->uninherited = std::move(uninherited);
		current_timing_velocity = (this->inherited.empty()) ? 1 : this->inherited.front()->get_velocity();
		current_beat_length = this->uninherited.front()->beat_length;

		constexpr int64_t START_TIME_OFFSET = 0;
		previous_time = timer.start_time = beatmap->stats.time.start_time - START_TIME_OFFSET;
	}

	// ::Render
	PlayingScreen::Render::Components::Components(Storage* storage) :
		map(storage, &Work::Render::Layers::playground->render_buffer),
		cursor(storage, &Work::Render::Layers::cursor->render_buffer),
		health_bar(storage, &Work::Render::Layers::hud->render_buffer),
		score(storage, &Work::Render::Layers::hud->render_buffer)
	{
	}
	PlayingScreen::Render::Render(
		const Game::Beatmap::Beatmap& beatmap,
		const Core& logic_core) :
		components(&storage)
	{
		//! Map
		components.map.collection = storage.insert(
			Utilities::Container::get_last_element_iterator(storage),
			std::make_shared<Playing::Mapset::Mapset>(*Work::Render::Textures::skin, beatmap)
		);
		components.map.render_item = Work::Render::Layers::playground->render_buffer.add_collection(components.map.collection->get()); // Thêm vào render_buffer ở đây nè :D
		components.map.collection->get()->render_range = { {-5, 5} };

		//! Cursor
		components.cursor.collection = storage.insert(
			Utilities::Container::get_last_element_iterator(storage),
			std::make_shared<Playing::Cursor::Cursor>(*Work::Render::Textures::skin, &logic_core.key_stoke.direction.current_direction)
		);
		components.cursor.render_item = Work::Render::Layers::cursor->render_buffer.add_collection(components.cursor.collection->get()); // Thêm vào render_buffer ở đây nè :D

		//! HealthBar
		components.health_bar.collection = storage.insert(
			Utilities::Container::get_last_element_iterator(storage),
			std::make_shared<Playing::HealthBar::HealthBar>(*Work::Render::Textures::skin, &logic_core.health.value)
		);
		components.health_bar.render_item = Work::Render::Layers::hud->render_buffer.add_collection(components.health_bar.collection->get()); // Thêm vào render_buffer ở đây nè :D

		//! Score
		components.score.collection = storage.insert(
			Utilities::Container::get_last_element_iterator(storage),
			std::make_shared<Playing::Score::Score>(
				*Work::Render::Textures::skin, logic_core.score.get_score(),
				logic_core.score.counter.get_accuracy(),
				logic_core.score.get_current_combo())
		);
		components.score.render_item = Work::Render::Layers::hud->render_buffer.add_collection(components.score.collection->get()); // Thêm vào render_buffer ở đây nè :D
	}

	// ::Audio
	PlayingScreen::Audio::Audio(const Game::Beatmap::Beatmap& beatmap, const std::filesystem::path& beatmap_root)
		: beatmap(&beatmap)
	{
		/*const auto beatmap_audio_file = beatmap_root / beatmap.general.audio_file;
		Work::Audio::Memory::Beatmap::music.load(beatmap_audio_file, beatmap_audio_file.filename().string());
		Work::Audio::Memory::Beatmap::effect.load(beatmap_root);*/
	}

	// ::
	PlayingScreen::PlayingScreen(const std::filesystem::path& beatmap) :
		beatmap(std::make_unique<const Game::Beatmap::Beatmap>(beatmap)),
		logic_core(this),
		render(*this->beatmap, logic_core),
		audio(*this->beatmap, beatmap.parent_path())
	{
		logic_core.timer.resume();
	}
}
