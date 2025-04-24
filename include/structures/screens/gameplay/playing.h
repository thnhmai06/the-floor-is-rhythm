// ♪ https://youtu.be/i0fw1thgnK0 <3

#pragma once
#include "config.h"
#include "structures/screens/screen.h"
#include "structures/action/event/input.h"
#include "structures/audio/mixer.h"
#include "structures/action/time.h"
#include "structures/game/beatmap.h"

namespace Structures::Screens::Gameplay
{
	using namespace Render::Objects;
	using Render::Layers::Layer;
	using Action::Event::Input::KeyboardEventList;

	struct PlayingScreen : private Screen
	{
		std::unique_ptr<const Game::Beatmap::Beatmap> beatmap;
		float mod_multiplier = 1.0f;

		//! Core
		struct Core final
		{
		private:
			const Game::Beatmap::Beatmap* beatmap;
			Game::Beatmap::Beatmap::FloorActionQueue floor;
			Game::Beatmap::Beatmap::SliderActionQueue slider;

		public:
			Action::Time::Timer timer;
			struct Keystroke
			{
				struct KeyCounter
				{
					SDL_Scancode target;
					unsigned long count = 0;
					uint8_t recently_pressed_num = 0; // => tính được cps
					bool is_hold = false;

					void update(const KeyboardEventList& events);
					void reset();

					explicit KeyCounter(const SDL_Scancode& target);
				};

				struct Direction
				{
					Types::Game::Direction::Direction current_direction = Types::Game::Direction::Direction::RIGHT;

					KeyCounter right{ Config::UserConfig::KeyBinding::Direction::right };
					KeyCounter left{ Config::UserConfig::KeyBinding::Direction::left };
					KeyCounter up{ Config::UserConfig::KeyBinding::Direction::up };
					KeyCounter down{ Config::UserConfig::KeyBinding::Direction::down };

					void update(const KeyboardEventList& events);
					void reset();
				} direction;
				struct Click
				{
					KeyCounter k1{ Config::UserConfig::KeyBinding::Click::k1 };
					KeyCounter k2{ Config::UserConfig::KeyBinding::Click::k2 };

					void reset();

					[[nodiscard]] uint16_t get_recently_pressed_num() const;
					[[nodiscard]] bool is_hold() const;
					void update(const KeyboardEventList& events);
				} click;

				void reset();
				void update(const KeyboardEventList& events);
			} key_stoke;
			struct Score
			{
			private:
				// beatmap
				const Game::Beatmap::Beatmap::Stats* beatmap_stats;

				// gameplay
				float score = 0;
				unsigned long max_combo = 0;
				unsigned long current_combo = 0;
				float mod_multiplier = 1.0f;

				float update_score();

			public:
				struct SliderScore
				{
					std::unordered_set<float> completed_ticks;

				private:
					std::weak_ptr<const Game::Beatmap::HitObjects::Slider::Action> slider_action;
					[[nodiscard]] uint16_t get_bonus_score() const;

				public:
					float check_and_add_slider_tick_score(const int64_t& current_time, bool is_hold, Score& score, const float& current_input_latency = 0);
					float add_final_bonus_score(const int64_t& current_time, Score& score) const;

					SliderScore(const std::shared_ptr<const Game::Beatmap::HitObjects::Slider::Action>& slider_action);
				};
				std::unordered_map<std::shared_ptr<const Game::Beatmap::HitObjects::Slider::Action>, SliderScore> current_slider;

				struct ScoreCounter
				{
				private:
					unsigned long count_300 = 0;
					unsigned long count_100 = 0;
					unsigned long count_50 = 0;
					unsigned long count_slider_tick = 0;
					unsigned long count_miss = 0;

				public:
					[[nodiscard]] float get_accuracy() const;
					[[nodiscard]] unsigned long get_elapsed_objects_num() const;
					void add_count(const uint16_t& score, const unsigned long& num = 1);
					void reset();
				} count;

				float add_floor_score(const uint16_t& score);

				[[nodiscard]] float get_score() const { return score; }
				[[nodiscard]] unsigned long get_current_combo() const { return current_combo; }
				[[nodiscard]] unsigned long get_max_combo() const { return max_combo; }
				[[nodiscard]] float get_mod_multiplier() const { return mod_multiplier; }

				explicit Score(const Game::Beatmap::Beatmap& beatmap, const float& mod_multiplier);
			} score;
			struct Health
			{
			private:
				const Game::Beatmap::Metadata::CalculatedDifficulty::HealthPoint* diff_hp;
				bool no_fail = false;

			public:
				float hp = 1;
				bool is_pause = false;

				bool update(const int16_t& note_score, const unsigned long& current_combo); // -> return: người chơi có fail không?

				explicit Health(const Game::Beatmap::Metadata::CalculatedDifficulty::HealthPoint* diff_hp, bool no_fail = false);
				explicit Health(const Game::Beatmap::Beatmap& beatmap, bool no_fail = false);
			} health;

			[[nodiscard]] bool is_paused() const { return timer.is_paused(); }
			void pause();
			void resume();
			void make_time_step(const KeyboardEventList& events, const float& current_input_latency = 0);
			explicit Core(
				const Game::Beatmap::Beatmap* beatmap, 
				const int64_t& start_time = 0, 
				const float& mod_multiplier = 1.0f, 
				bool no_fail = false);
		} logic;

		//! Render
		struct Render final : Screen::Render
		{
			struct Components
			{
				Screen::Render::Item map;
				Screen::Render::Item cursor;
				Screen::Render::Item health_bar;
				//Screen::Render::Item score;
				//Screen::Render::Item combo;

				explicit Components(Storage* storage);
			} components;

			Render(
				const Types::Game::Direction::Direction* current_direction,
				const Game::Beatmap::Beatmap& beatmap);
		} render;

		struct Audio final
		{
			Structures::Audio::Mixer* mixer = nullptr;

		};

		explicit PlayingScreen(const char* beatmap_path);
	};
}