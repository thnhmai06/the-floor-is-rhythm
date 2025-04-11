#pragma once
#include "structures/screens/screen.h"
#include "structures/timer.h"
#include "parser/tfir/beatmap.h"
#include "structures/action.h"

namespace Structures::Screen::Gameplay
{
	using Render::RenderObjects::RenderObjectStorage, Render::Textures::TextureMemory, Render::Layers::Layer;
	using Timer::Timer, Action::KeyObserver;

	struct PlayingScreen : private Screens::Screen
	{
		std::unique_ptr<const Mapset> mapset;
		float mod_multiplier = 1.0f;

		struct Game final
		{
			const Mapset* mapset = nullptr;
			//std::queue<GameObjects::HitObjects::HitObject*> active_hit_object;

		private:
			Template::Game::Direction::Direction require_direction = Template::Game::Direction::Direction::RIGHT;
			Timer timer;
			KeyObserver key_observer;

		public:
			uint8_t health = 200;
			Template::Game::Direction::Direction current_direction = Template::Game::Direction::Direction::RIGHT;
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
					KeyCounter right{ UserConfig::KeyBinding::Direction::right };
					KeyCounter left{ UserConfig::KeyBinding::Direction::left };
					KeyCounter up{ UserConfig::KeyBinding::Direction::up };
					KeyCounter down{ UserConfig::KeyBinding::Direction::down };

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
					KeyCounter k1{ UserConfig::KeyBinding::Click::k1 };
					KeyCounter k2{ UserConfig::KeyBinding::Click::k2 };

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
				const float* mod_multiplier;

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

				explicit Score(const Mapset& beatmap, const float* mod_multiplier);
			} score{ *mapset, &mod_multiplier };

			[[nodiscard]] bool is_paused() const { return timer.is_paused(); }
			void pause() { timer.pause(); }
			void resume() { timer.resume(); }
			void make_time_step()
			{
				key_stoke.update(key_observer);

				//TODO: object interaction
			}
			explicit Game(const Mapset* mapset, const int64_t& start_time = 0);
		} game;
		struct Render : Screen::Render
		{
			struct Components
			{
				Screen::Render::Item map_set;
				Screen::Render::Item cursor;

				Components(
					RenderObjectStorage* storage,
					Layer* playground_layer,
					Layer* cursor_layer);
			} components;

			Render(
				const Template::Game::Direction::Direction* current_direction,
				Layer& playground_layer,
				Layer& cursor_layer,
				const TextureMemory& skin,
				const Mapset& mapset);
		} render;

		explicit PlayingScreen(const char* beatmap_path);
	};
}