// ♪ https://youtu.be/i0fw1thgnK0 <3
#pragma once
#include <filesystem>
#include "config.h"
#include "playing/mapset.h"
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
	using Action::Event::Input::EventList;

	struct PlayingScreen : private Screen
	{
		std::unique_ptr<const Game::Beatmap::Mapset> beatmap;
		float mod_multiplier = 1.0f;

		//! Core
		struct Core final
		{
		protected:
			bool is_started = false;

		private:
			const PlayingScreen* playing_screen;
			float current_timing_velocity, current_beat_length;

			int64_t previous_time;
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
				struct Click
				{
					KeyCounter k1{ Config::UserConfig::KeyBinding::k1 };
					KeyCounter k2{ Config::UserConfig::KeyBinding::k2 };

					void reset();

					[[nodiscard]] uint16_t get_recently_clicked_num() const;
					[[nodiscard]] bool is_hold() const;
					void update(const KeyboardEventList& events);
				} click;

				void reset();
				void update(const KeyboardEventList& events);
			} key_stoke;
			struct Score
			{
			private:
				// core
				const Keystroke* key_stoke;

				// beatmap
				const Game::Beatmap::Mapset* beatmap;

				// gameplay
				float score = 0;
				unsigned long max_combo = 0;
				unsigned long current_combo = 0;
				float mod_multiplier = 1.0f;

				void update_score();

			public:
				struct ScoreCounter
				{
				private:
					float accuracy = 1;
					unsigned long count_300 = 0;
					unsigned long count_100 = 0;
					unsigned long count_50 = 0;
					unsigned long count_slider_tick = 0;
					unsigned long count_miss = 0;

				public:
					[[nodiscard]] const float* get_accuracy() const { return &accuracy; }
					[[nodiscard]] const unsigned long* get_count_300() const { return &count_300; }
					[[nodiscard]] const unsigned long* get_count_100() const { return &count_100; }
					[[nodiscard]] const unsigned long* get_count_50() const { return &count_50; }
					[[nodiscard]] const unsigned long* get_count_slider_tick() const { return &count_slider_tick; }
					[[nodiscard]] const unsigned long* get_count_miss() const { return &count_miss; }
					[[nodiscard]] unsigned long get_elapsed_objects_num() const;
					void update_accuracy();
					void add_count(const uint16_t& score, const unsigned long& num = 1);
					void reset();
				} counter;
				struct SliderScore
				{
					float last_uncompleted_tick_slider_time = 0;
					unsigned long completed_ticks_num = 0;
					std::weak_ptr<const Game::Beatmap::HitObjects::Slider::ActionInfo> action_info;

					explicit SliderScore(const std::shared_ptr<const Game::Beatmap::HitObjects::Slider::ActionInfo>& slider_action);
				};
				using ActiveSliders = std::unordered_map<std::shared_ptr<const Game::Beatmap::HitObjects::Slider::ActionInfo>, SliderScore>;
				ActiveSliders active_sliders;
				float check_and_add_floor_score(const int64_t& current_time, const float& input_latency = 0);
				float check_and_add_slider_score(const int64_t& current_time, const float& input_latency = 0);

				[[nodiscard]] const float* get_score() const;
				[[nodiscard]] const unsigned long* get_current_combo() const;
				[[nodiscard]] const unsigned long* get_max_combo() const;
				[[nodiscard]] const float* get_mod_multiplier() const;

				explicit Score(Core* core, const float& mod_multiplier);
			} score;
			struct Health
			{
			private:
				const Game::Beatmap::Metadata::CalculatedDifficulty::HPDrainRate* diff_hp;
				bool no_fail = false;

			public:
				float value = 1;
				bool is_pause = false;

				bool update(const int16_t& note_score, const unsigned long& current_combo); // -> return: người chơi có fail không?

				explicit Health(const Game::Beatmap::Metadata::CalculatedDifficulty::HPDrainRate* diff_hp, bool no_fail = false);
				explicit Health(const Game::Beatmap::Mapset* beatmap, bool no_fail = false);
			} health;

			[[nodiscard]] bool is_paused() const;
			void pause();
			void resume();
			void make_time_step(const EventList& events, const float& input_latency = 0);

			explicit Core(
				const PlayingScreen* playing_screen, 
				const int64_t& start_time = 0, 
				const float& mod_multiplier = 1.0f, 
				bool no_fail = false);
		} logic_core;

		//! Render
		struct Render final : Screen::Render
		{
			struct Components
			{
				Screen::Render::Item mapset;
				Screen::Render::Item cursor;
				Screen::Render::Item health_bar;
				Screen::Render::Item score;

				explicit Components(Storage* storage);
			} components;

			Playing::Mapset::Mapset::MovingPosMap moving_pos;
			void move(const int64_t& current_time, const float& distance) const;

			Render(
				const Game::Beatmap::Mapset& beatmap,
				const Core& logic_core);
		} render;

		//! Audio
		struct Audio final
		{
			const Game::Beatmap::Mapset* beatmap;

			explicit Audio(const Game::Beatmap::Mapset& beatmap, const std::filesystem::path& beatmap_root);
		} audio;

		explicit PlayingScreen(const std::filesystem::path& beatmap);
	};
}