#pragma once
#include "config.h"
#include "structures/screens/screen.h"
#include "structures/action.h"
#include "structures/game/beatmap/beatmap.h"

namespace Structures::Screens::Gameplay
{
	using Render::Objects::Storage, Render::Textures::TextureMemory, Render::Layers::Layer;
	using Action::Key::KeyObserver;

	struct PlayingScreen : private Screen
	{
		std::unique_ptr<const Game::Beatmap::Beatmap> beatmap;
		float mod_multiplier = 1.0f;

		struct Logic final
		{
			const Game::Beatmap::Beatmap* beatmap = nullptr;
			//std::queue<Logic::HitObjects::HitObject*> active_hit_object;

		private:
			Types::Game::Direction::Direction require_direction = Types::Game::Direction::Direction::RIGHT;
			Action::Time::Timer timer;
			KeyObserver key_observer;

		public:
			uint8_t health = 200;
			Types::Game::Direction::Direction current_direction = Types::Game::Direction::Direction::RIGHT;
			struct Keystroke
			{
				struct KeyCounter
				{
					SDL_Scancode target;
					unsigned long count = 0;
					bool is_hold = false;

					void update(KeyObserver& observer)
					{
						const auto& [target_is_pressed, target_is_hold] = observer[target];
						if (target_is_pressed) count++;
						this->is_hold = target_is_hold;
					}
					void reset() { count = 0; is_hold = false; }
					explicit KeyCounter(const SDL_Scancode& target) : target(target) {}
				};
				struct
				{
					KeyCounter right{ Config::UserConfig::KeyBinding::Direction::right };
					KeyCounter left{ Config::UserConfig::KeyBinding::Direction::left };
					KeyCounter up{ Config::UserConfig::KeyBinding::Direction::up };
					KeyCounter down{ Config::UserConfig::KeyBinding::Direction::down };

					void reset()
					{
						right.reset();
						left.reset();
						up.reset();
						down.reset();
					}
					void update(KeyObserver& observer)
					{
						right.update(observer);
						left.update(observer);
						up.update(observer);
						down.update(observer);
					}
				} direction;
				struct
				{
					KeyCounter k1{ Config::UserConfig::KeyBinding::Click::k1 };
					KeyCounter k2{ Config::UserConfig::KeyBinding::Click::k2 };

					void reset()
					{
						k1.reset();
						k2.reset();
					}
					void update(KeyObserver& observer)
					{
						k1.update(observer);
						k2.update(observer);
					}
				} click;

				void reset()
				{
					direction.reset();
					click.reset();
				}
				void update(KeyObserver& observer)
				{
					direction.update(observer);
					click.update(observer);
				}
			} key_stoke;
			struct Score
			{
			private:
				unsigned long total_objects_num = 1;
				unsigned long total_combo = 1;
				float mod_multiplier = 1.0f;

			public:
				unsigned long score = 0;
				unsigned long max_combo = 0;
				struct Count
				{
					unsigned long count_300 = 0;
					unsigned long count_100 = 0;
					unsigned long count_50 = 0;
					unsigned long count_slider_tick = 0;
					unsigned long count_miss = 0;

					[[nodiscard]] float get_accuracy() const;
					[[nodiscard]] unsigned long get_elapsed_objects_num() const;
				} count;
				unsigned long combo = 0;

				unsigned long update_score();

				explicit Score(const Game::Beatmap::Beatmap& beatmap, const float& mod_multiplier);
			} score;

			[[nodiscard]] bool is_paused() const { return timer.is_paused(); }
			void pause() { timer.pause(); }
			void resume() { timer.resume(); }
			void make_time_step()
			{
				key_stoke.update(key_observer);

				//TODO: object interaction
			}
			explicit Logic(const Game::Beatmap::Beatmap* beatmap, const int64_t& start_time = 0, const float& mod_multiplier = 1.0f);
		} game;
		struct Render : Screen::Render
		{
			struct Components
			{
				Screen::Render::Item map_set;
				Screen::Render::Item cursor;

				Components(
					Storage* storage,
					Layer* playground_layer,
					Layer* cursor_layer);
			} components;

			Render(
				const Types::Game::Direction::Direction* current_direction,
				Layer& playground_layer,
				Layer& cursor_layer,
				const TextureMemory& skin,
				const Game::Beatmap::Beatmap& beatmap);
		} render;

		explicit PlayingScreen(const char* beatmap_path);
	};
}