// ReSharper disable CppClangTidyClangDiagnosticShadow
#include "structures/screens/gameplay/playing.h" // Header
#include <ranges>
#include "structures/render/layers/objects/gameplay/beatmap.h"
#include "structures/render/layers/objects/gameplay/cursor.h"
#include "work/render/layers.h"
#include "work/render/textures.h"
#include "utilities.hpp"

//! PLayingScreen
namespace Structures::Screens::Gameplay
{
	// ::Logic::Score::count
	float PlayingScreen::Logic::Score::ScoreCounter::get_accuracy() const
	{
		if (count_300 + count_100 + count_50 + count_slider_tick + count_miss == 0) return 100.00f; // tránh chia 0

		// chia 10 để giảm số cần nhớ
		constexpr uint8_t SCORE_PERFECT = 30; // 300
		constexpr uint8_t SCORE_GREAT = 10; // 100
		constexpr uint8_t SCORE_BAD = 5; // 50
		constexpr uint8_t SCORE_SLIDER_TICK = 1; // 10

		const unsigned long total = count_300 * SCORE_PERFECT + count_100 * SCORE_GREAT + count_50 * SCORE_BAD + count_slider_tick * SCORE_SLIDER_TICK;
		const unsigned long total_if_perfect = (count_300 + count_100 + count_50 + count_miss) * SCORE_PERFECT + count_slider_tick * SCORE_SLIDER_TICK;
		return static_cast<float>(total) / static_cast<float>(total_if_perfect);
	}

	void PlayingScreen::Logic::Score::ScoreCounter::reset()
	{
		count_300 = count_100 = count_50 = count_slider_tick = count_miss = 0;
	}

	unsigned long PlayingScreen::Logic::Score::ScoreCounter::get_elapsed_objects_num() const
	{
		return count_300 + count_100 + count_50 + count_miss;
	}
	void PlayingScreen::Logic::Score::ScoreCounter::add_count(const uint16_t& score, const unsigned long& num)
	{
		switch (score)
		{
		case 300:
			count_300 += num;
			break;
		case 100:
			count_100 += num;
			break;
		case 50:
			count_50 += num;
			break;
		case 10:
			count_slider_tick += num;
			break;
		default:
			count_miss += num;
			break;
		}
	}

	// ::Logic::Score::SliderScore
	uint16_t PlayingScreen::Logic::Score::SliderScore::get_bonus_score() const
	{
		const auto slider_action = this->slider_action.lock();

		if (completed_ticks.empty()) return 0;
		if (completed_ticks.size() == slider_action->tick_num)
			return 300;
		if (completed_ticks.size() >= static_cast<unsigned long>(ceil(static_cast<float>(slider_action->tick_num) / 2)))
			return 100;
		return 50;
	}
	float PlayingScreen::Logic::Score::SliderScore::check_and_add_slider_tick_score(
		const int64_t& current_time, const bool is_hold,
		Score& score, const float& current_input_latency)
	{
		const auto slider_action = this->slider_action.lock();
		const float current_slider_time = static_cast<float>(current_time - slider_action->time);
		const auto end_slider_time = static_cast<float>(slider_action->end_time - slider_action->time);

		float nearest_tick_slider_time;
		if (current_slider_time < 0)
			nearest_tick_slider_time = 0;
		else
		{
			nearest_tick_slider_time = std::min(
				std::round(current_slider_time / slider_action->tick_length) * slider_action->tick_length,
				end_slider_time
			);

			while (completed_ticks.contains(nearest_tick_slider_time))
			{
				nearest_tick_slider_time += slider_action->tick_length; // vì không thể đi ngược lại, chỉ có thể đi tiến
				if (nearest_tick_slider_time > end_slider_time)
				{
					nearest_tick_slider_time = end_slider_time;
					break;
				}
			}
		}

		const auto previous_score = score.get_score();
		if (Utilities::Math::in_range(nearest_tick_slider_time, nearest_tick_slider_time + current_input_latency, current_slider_time))
		{
			if (is_hold && !completed_ticks.contains(nearest_tick_slider_time))
			{
				completed_ticks.insert(nearest_tick_slider_time);
				score.count.add_count(10);
				score.current_combo++;
			}
			else score.current_combo = 0;
		}
		else return 0;

		const auto current_score = score.update_score();
		return current_score - previous_score;
	}
	float PlayingScreen::Logic::Score::SliderScore::add_final_bonus_score(const int64_t& current_time, Score& score) const
	{
		const auto previous_score = score.get_score();
		score.count.add_count(get_bonus_score());
		const auto current_score = score.update_score();
		return current_score - previous_score;
	}
	PlayingScreen::Logic::Score::SliderScore::SliderScore(const std::shared_ptr<const Game::Beatmap::HitObjects::Slider::Action>& slider_action)
		: slider_action(slider_action)
	{
	}

	// ::Logic::Score
	float PlayingScreen::Logic::Score::update_score()
	{
		// Score = ((700000 * max_combo / total_combo) + (300000 * accuracy ^ 10 * elapsed_objects / total_objects)) * mod_multiplier

		constexpr float BASE_COMBO = 700000;
		constexpr float BASE_ACCURACY = 300000;

		const float accuracy = count.get_accuracy();
		const float elapsed_objects = static_cast<float>(count.get_elapsed_objects_num());

		const float combo_score = BASE_COMBO * static_cast<float>(max_combo) / static_cast<float>(beatmap_stats->count.total_combo);
		const float accuracy_score = BASE_ACCURACY * std::powf(accuracy, 10) * elapsed_objects / static_cast<float>(beatmap_stats->get_total_objects_num());
		const float score_no_multiplier = (combo_score + accuracy_score) * mod_multiplier;
		return this->score = score_no_multiplier * mod_multiplier;
	}
	float PlayingScreen::Logic::Score::add_floor_score(const uint16_t& score)
	{
		const auto previous_score = get_score();

		count.add_count(score);
		if (score == 0) current_combo = 0;
		else current_combo++;
		max_combo = std::max(current_combo, max_combo);

		const auto current_score = update_score();
		return current_score - previous_score;
	}
	PlayingScreen::Logic::Score::Score(const Game::Beatmap::Beatmap& beatmap, const float& mod_multiplier)
		: beatmap_stats(&beatmap.stats), mod_multiplier(mod_multiplier)
	{
	}

	// ::Logic::Keystroke::KeyCounter
	void PlayingScreen::Logic::Keystroke::KeyCounter::update(const KeyboardEventList& events)
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
	void PlayingScreen::Logic::Keystroke::KeyCounter::reset()
	{
		count = recently_pressed_num = 0;
		is_hold = false;
	}
	PlayingScreen::Logic::Keystroke::KeyCounter::KeyCounter(const SDL_Scancode& target) : target(target)
	{
	}

	// ::Logic::Keystroke::Direction
	void PlayingScreen::Logic::Keystroke::Direction::update(const KeyboardEventList& events)
	{
		// Cập nhật lần cuối cùng thay đổi direction trong events
		for (auto event = events.rbegin(); event != events.rend(); ++event)
		{
			if (!event->repeat) continue;
			if (event->scancode == right.target)
				current_direction = Types::Game::Direction::Direction::RIGHT;
			else if (event->scancode == left.target)
				current_direction = Types::Game::Direction::Direction::LEFT;
			else if (event->scancode == up.target)
				current_direction = Types::Game::Direction::Direction::UP;
			else if (event->scancode == down.target)
				current_direction = Types::Game::Direction::Direction::DOWN;
			break;
		}

		right.update(events);
		left.update(events);
		up.update(events);
		down.update(events);
	}
	void PlayingScreen::Logic::Keystroke::Direction::reset()
	{
		right.reset();
		left.reset();
		up.reset();
		down.reset();
	}

	// ::Logic::Keystroke::Click
	void PlayingScreen::Logic::Keystroke::Click::reset()
	{
		k1.reset();
		k2.reset();
	}
	uint16_t PlayingScreen::Logic::Keystroke::Click::get_recently_pressed_num() const
	{
		return k1.recently_pressed_num + k2.recently_pressed_num;
	}
	bool PlayingScreen::Logic::Keystroke::Click::is_hold() const
	{
		return k1.is_hold || k2.is_hold;
	}
	void PlayingScreen::Logic::Keystroke::Click::update(const KeyboardEventList& events)
	{
		k1.update(events);
		k2.update(events);
	}

	// ::Logic::Keystroke
	void PlayingScreen::Logic::Keystroke::reset()
	{
		direction.reset();
		click.reset();
	}
	void PlayingScreen::Logic::Keystroke::update(const KeyboardEventList& events)
	{
		direction.update(events);
		click.update(events);
	}

	// ::Logic
	void PlayingScreen::Logic::pause() { timer.pause(); }
	void PlayingScreen::Logic::resume() { timer.resume(); }
	void PlayingScreen::Logic::make_time_step(const KeyboardEventList& events, const float& current_input_latency)
	{
		key_stoke.update(events);
		const auto current_time = timer.get_time();

		//! Tính điểm
		const auto& od = beatmap->calculated_difficulty.od;

		// Floor
		while (!floor.empty() && floor.front()->time <= current_time)
		{
			const auto current_floor = Utilities::Container::get_front_and_pop(floor);
			score.add_floor_score(
				od.get_score(current_time, current_floor->time, current_floor->direction, key_stoke.direction.current_direction)
			);
		}

		// Slider
		while (!slider.empty() && Utilities::Math::in_range(slider.front()->time, slider.front()->end_time, current_time))
		{
			if (const auto current_slider = Utilities::Container::get_front_and_pop(slider);
				!score.current_slider.contains(current_slider)) // cho chắc
				score.current_slider.insert({ current_slider, Score::SliderScore{current_slider} });
		}
		for (auto& [slider_action, slider_score] : score.current_slider)
		{
			slider_score.check_and_add_slider_tick_score(current_time, key_stoke.click.is_hold(), score, current_input_latency);
			if (current_time > slider_action->end_time)
			{
				slider_score.add_final_bonus_score(current_time, score);
				score.current_slider.erase(slider_action);
			}
		}
	}

	PlayingScreen::Logic::Logic(const Game::Beatmap::Beatmap* beatmap, const int64_t& start_time, const float& mod_multiplier)
		: beatmap(beatmap), timer(start_time), score(*beatmap, mod_multiplier)
	{
		constexpr int64_t START_TIME_OFFSET = 3000; // 3s
		timer.start_time = beatmap->stats.time.start_time - START_TIME_OFFSET;

		auto [floor, slider] = beatmap->make_action_queue();
		this->floor = std::move(floor);
		this->slider = std::move(slider);
	}

	// ::Render
	PlayingScreen::Render::Components::Components(
		Storage* storage,
		Layer* playground_layer, Layer* cursor_layer) :
		map_set(storage, &playground_layer->render_buffer),
		cursor(storage, &cursor_layer->render_buffer) {
	}
	PlayingScreen::Render::Render(
		const Types::Game::Direction::Direction* current_direction,
		Layer& playground_layer,
		Layer& cursor_layer,
		const TextureMemory& skin,
		const Game::Beatmap::Beatmap& beatmap) : components(&storage, &playground_layer, &cursor_layer)
	{
		//! Beatmap
		components.map_set.storage_item = storage.insert(
			Utilities::Container::get_last_element_iterator(storage),
			std::make_unique<Structures::Render::Objects::Gameplay::Beatmap::Collection>(skin, beatmap)
		);
		components.map_set.render_item = playground_layer.render_buffer.add_collection(components.map_set.storage_item->get()); // Thêm vào render_buffer ở đây nè :D
		//TODO: set render range
		components.map_set.storage_item->get()->render_range = { {0, 10} }; // for testing

		//! Cursor
		components.cursor.storage_item = storage.insert(
			Utilities::Container::get_last_element_iterator(storage),
			std::make_unique<Structures::Render::Objects::Gameplay::Cursor::Collection>(skin, current_direction)
		);
		components.cursor.render_item = cursor_layer.render_buffer.add_collection(components.cursor.storage_item->get()); // Thêm vào render_buffer ở đây nè :D
	}
	// total
	PlayingScreen::PlayingScreen(const char* beatmap_path) :
		beatmap(std::make_unique<const Game::Beatmap::Beatmap>(beatmap_path)),
		logic(beatmap.get()),
		render(&logic.key_stoke.direction.current_direction,
			*Work::Render::Layers::playground, *Work::Render::Layers::cursor,
			*Work::Render::Textures::skin, *this->beatmap)
	{
	}
}