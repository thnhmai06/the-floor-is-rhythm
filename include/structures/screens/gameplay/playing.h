#pragma once
#include "structures/screens/screen.h"
#include "structures/timer.h"
#include "parser/tfir/beatmap.h"
#include "structures/action.h"

namespace Structures::Screen::Gameplay
{
	using Render::RenderObjects::RenderObjectStorage, Render::Textures::TextureMemory, Render::Layers::Layer;

	struct PlayingScreen : private Screens::Screen
	{
		std::unique_ptr<const Mapset> mapset;
		GameObjects::HitObjects::HitObjects::iterator current_hit_object;

		struct Game final
		{
			const Mapset* mapset = nullptr;

		private:
			Template::Game::Direction::Direction require_direction = Template::Game::Direction::Direction::RIGHT;
			Timer timer;
			PlayingKeyQueue key_pressed;

		public:
			uint8_t health = 200;
			int64_t current_time = -2000; // ms
			struct Keystroke
			{
				struct
				{
					unsigned long right = 0;
					unsigned long left = 0;
					unsigned long up = 0;
					unsigned long down = 0;

					void reset()
					{
						right = 0;  left = 0; up = 0; down = 0;
					}
					void count_for(const int& key)
					{
						if (key == UserConfig::KeyBinding::Direction::right) right++;
						else if (key == UserConfig::KeyBinding::Direction::left) left++;
						else if (key == UserConfig::KeyBinding::Direction::up) up++;
						else if (key == UserConfig::KeyBinding::Direction::down) down++;
					}
				} direction;
				struct
				{
					unsigned long k1 = 0;
					unsigned long k2 = 0;

					void reset()
					{
						k1 = 0; k2 = 0;
					}
					void count_for(const int& key)
					{

						if (key == UserConfig::KeyBinding::Click::k1) k1++;
						else if (key == UserConfig::KeyBinding::Click::k2) k2++;
					}
				} click;

				void count_for(const int& key)
				{
					if (UserConfig::KeyBinding::Direction::is_direction(key))
						direction.count_for(key);
					else if (UserConfig::KeyBinding::Click::is_click(key))
						click.count_for(key);
				}
				void reset()
				{
					direction.reset();
					click.reset();
				}
			} key_stoke;
			Template::Game::Direction::Direction current_direction = Template::Game::Direction::Direction::RIGHT;

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

				explicit Score(const Mapset& beatmap, const float& mod_multiplier = 1.0f);
			};

			[[nodiscard]] bool is_paused() const { return timer.is_paused(); }
			void pause() { timer.pause(); }
			void resume() { timer.resume(); }

			void make_time_step()
			{
				current_time = timer.get_time();
				while (!key_pressed.empty())
				{
					const auto key = key_pressed.front();
					key_pressed.pop();
					key_stoke.count_for(key);
					if (UserConfig::KeyBinding::Direction::is_direction(key))
					{
                        if (key == UserConfig::KeyBinding::Direction::right)
                        	current_direction = Template::Game::Direction::Direction::RIGHT;
                        else if (key == UserConfig::KeyBinding::Direction::left)
                            current_direction = Template::Game::Direction::Direction::LEFT;
                        else if (key == UserConfig::KeyBinding::Direction::up)
                            current_direction = Template::Game::Direction::Direction::UP;
                        else if (key == UserConfig::KeyBinding::Direction::down)
                            current_direction = Template::Game::Direction::Direction::DOWN;
					}

					// TODO: Xử lý key
					/*if (current_hit_object->first)*/
				}
			}

			Game(const Mapset* mapset);
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