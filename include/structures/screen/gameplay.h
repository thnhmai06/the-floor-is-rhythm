// ♪ https://youtu.be/i0fw1thgnK0 <3
#pragma once
#include <filesystem>
#include "structures/audio/mixer.h"
#include "structures/events/time/time.h"
#include "structures/screen/gameplay/keystroke.h"
#include "structures/screen/gameplay/cursor.h"
#include "structures/screen/gameplay/health.h"
#include "structures/screen/gameplay/score.h"
#include "structures/screen/gameplay/mapset.h"

namespace Structures::Screen::Gameplay
{
	using namespace Render::Object;

	struct PlayingScreen final
	{
	protected:
		std::unique_ptr<const Game::Beatmap::Mapset> mapset;
		float mod_multiplier = 1.0f;
		bool no_fail = false;
		bool auto_play = false;
		Events::Event::Buffer event_buffer;
		Events::Action::Buffer action_buffer;

	public:
		//! Logic
		struct Logic final
		{
		protected:
			PlayingScreen* playing_screen;
			bool is_started = false;

		private:
			void update_input(const int64_t& current_time, const Events::Event::Input::SdlEvents& events);
			void update_render(const int64_t& current_time) const;
			void update_timing(const int64_t& current_time);
			bool update_score_and_health(const Types::Game::Gameplay::NoteScore& obj_score);
			bool update_object(const int64_t& current_time, uint16_t& click_left, uint16_t& click_right);
			void update_event_and_action(const int64_t& current_time) const;

		public:
			struct Current
			{
				Mapset::Render::Mapset::RenderScripts::const_iterator object_script;
				Game::Beatmap::Hitsound::TimingSample timing_sample;
				const Game::Beatmap::Hitsound::HitSampleType* beatmap_sample;

				explicit Current(const Game::Beatmap::Mapset& mapset);
			} current;

			struct System
			{
				Events::Time::Timer timer;
				KeyStroke::Logic::Keystroke key_stroke;
				Score::Logic::Score score;
				Health::Logic::Health health;

				System(const Game::Beatmap::Mapset& mapset, const bool* no_fail, const float* mod_multiplier);
			} system;


			[[nodiscard]] const bool* is_paused() const;
			void pause();
			void resume();
			void make_time_step(const Events::Event::Input::SdlEvents& events);

			explicit Logic(
				PlayingScreen* playing_screen,
				const float* mod_multiplier,
				const bool* no_fail);
		} logic;

		//! Render
		struct Render final
		{
			PlayingScreen* playing_screen;
			std::shared_ptr<Cursor::Render::Cursor> cursor;
			std::shared_ptr<Health::Render::Health> health;
			std::shared_ptr<Mapset::Render::Mapset> mapset;
			std::shared_ptr<Score::Render::Score> score;
			std::unique_ptr<Game::Beatmap::Event::EventObjects> storyboard;

			explicit Render(
				PlayingScreen* playing_screen,
				const Memory& memory, bool load_storyboard);
		} render;

		//! Audio
		struct Audio final
		{
			PlayingScreen* playing_screen;
			Structures::Audio::Mixer* mixer;
			Structures::Audio::MusicMemory* memory_music;
			Structures::Audio::EffectMemory* memory_effect;

			void check_and_play_sound(
				const int64_t& current_time,
				const Game::Beatmap::HitObjects::Floor& floor,
				const Types::Game::Gameplay::NoteScore& score,
				const Game::Beatmap::Hitsound::TimingSample& timing_sample, 
				const Game::Beatmap::Hitsound::HitSampleType& beatmap_sample) const;

			explicit Audio(PlayingScreen* playing_screen,
				const std::filesystem::path& mapset_root,
				Structures::Audio::Mixer* mixer,
				Structures::Audio::MusicMemory* memory_music,
				Structures::Audio::EffectMemory* memory_effect);
		} audio;

		explicit PlayingScreen(const std::filesystem::path& mapset_path, const float& mod_multiplier, bool load_storyboard, bool no_fail, bool auto_play = false);
	};
}