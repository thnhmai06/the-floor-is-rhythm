#pragma once
#include "config.h"
#include "structures/screens/screen.h"
#include "structures/action.h"
#include "structures/game/beatmap/beatmap.h"

namespace Structures::Screens::Gameplay
{
	using Render::Objects::Storage, Render::Textures::TextureMemory, Render::Layers::Layer;
	using Action::Event::KeyboardEventList;

	struct PlayingScreen : private Screen
	{
		std::unique_ptr<const Game::Beatmap::Beatmap> beatmap;
		float mod_multiplier = 1.0f;

		struct Logic final
		{
		private:
			const Game::Beatmap::Beatmap* beatmap = nullptr;
			Types::Game::Direction::Direction required_direction = Types::Game::Direction::Direction::RIGHT;
			Game::Beatmap::HitObjects::HitObjects::const_iterator current_hit_object = beatmap->hit_objects.cbegin();
			Action::Time::Timer timer;

			void update_required_direction()
			{
				if (current_hit_object == beatmap->hit_objects.cend()) return;
				required_direction += current_hit_object->second.get_rotation();
			}

		public:
			uint8_t health = 200;
			struct Keystroke
			{
				struct KeyCounter
				{
					SDL_Scancode target;
					unsigned long count = 0;
					uint8_t recently_pressed_num = 0; // => tính được cps
					bool is_hold = false;

					void update(const KeyboardEventList& events)
					{
						recently_pressed_num = 0;
						for (const auto& event: events)
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
					void reset() { count = recently_pressed_num = 0; is_hold = false; }
					explicit KeyCounter(const SDL_Scancode& target) : target(target) {}
				};
				struct
				{
					KeyCounter right{ Config::UserConfig::KeyBinding::Direction::right };
					KeyCounter left{ Config::UserConfig::KeyBinding::Direction::left };
					KeyCounter up{ Config::UserConfig::KeyBinding::Direction::up };
					KeyCounter down{ Config::UserConfig::KeyBinding::Direction::down };
					Types::Game::Direction::Direction current_direction = Types::Game::Direction::Direction::RIGHT;

					void reset()
					{
						right.reset();
						left.reset();
						up.reset();
						down.reset();
					}
					void update(const KeyboardEventList& events)
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
					[[nodiscard]] uint16_t get_recently_pressed_num() const
					{
						return k1.recently_pressed_num + k2.recently_pressed_num;
					}
					void update(const KeyboardEventList& events)
					{
						k1.update(events);
						k2.update(events);
					}
				} click;

				void reset()
				{
					direction.reset();
					click.reset();
				}
				void update(const KeyboardEventList& events)
				{
					direction.update(events);
					click.update(events);
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
			void make_time_step(const KeyboardEventList& events)
			{
				key_stoke.update(events);

				const auto current_time = timer.get_time();
				const auto& od = beatmap->calculated_difficulty.od;

				if (current_hit_object != beatmap->hit_objects.end())
				{
					if (const auto current_required_time = current_hit_object->second.get_time();
						!(current_time <= current_required_time && od.get_score(current_time, current_required_time) < 0))
						// Không phải là chưa tới thời gian bấm
					{
						if (required_direction == key_stoke.direction.current_direction)
						{

						}
					}
				}
			}
			explicit Logic(const Game::Beatmap::Beatmap* beatmap, const int64_t& start_time = 0, const float& mod_multiplier = 1.0f);
		} logic;
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