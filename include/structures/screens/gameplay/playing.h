#pragma once
#include "structures/screens/screen.h"
#include "structures/timer.h"
#include "parser/tfir/beatmap.h"

namespace Structures::Render::Screen::Gameplay
{
	struct PlayingScreen : private Screens::Screen
	{
		std::unique_ptr<const BeatmapFile> beatmap;
		
		struct Render : Screen::Render
		{
			struct Components
			{
				Screen::Render::Item map_set;
				Screen::Render::Item cursor;

				Components(
					RenderObjects::RenderObjectStorage* storage,
					Layers::Layer* playground_layer, 
					Layers::Layer* cursor_layer);
			} components;

			Render(
				const Template::Game::Direction::Direction* current_direction,
				Layers::Layer& playground_layer,
				Layers::Layer& cursor_layer,
				const Textures::TextureMemory& skin,
				const BeatmapFile& beatmap);
		} render;
		struct Game final
		{
		private:
			Template::Game::Direction::Direction require_direction = Template::Game::Direction::Direction::RIGHT;
			GameObjects::HitObjects::HitObjects::iterator current_hit_object;
			Timer timer;

		public:
			uint8_t health = 200;
			int64_t current_time = -2000; // ms
			struct Count
			{
				unsigned long count_300 = 0;
				unsigned long count_100 = 0;
				unsigned long count_50 = 0;
				unsigned long count_miss = 0;

				[[nodiscard]] float get_accuracy() const;
			} count;
			struct Keystroke
			{
				struct
				{
					unsigned long right = 0;
					unsigned long left = 0;
					unsigned long up = 0;
					unsigned long down = 0;
				} direction;
				struct
				{
					unsigned long k1 = 0;
					unsigned long k2 = 0;
				} click;
			} key_stoke;
			unsigned long long score = 0;
			unsigned long combo = 0;
			unsigned long max_combo = 0;
			Template::Game::Direction::Direction current_direction = Template::Game::Direction::Direction::RIGHT;

			[[nodiscard]] bool is_paused() const { return timer.is_paused(); }
			void pause() { timer.pause(); }
			void resume() { timer.resume(); }

			void make_time_step()
			{
				current_time = timer.get_time();
			}

			Game() = default;
		} game;

		explicit PlayingScreen(const char* beatmap_path);
	};
}
