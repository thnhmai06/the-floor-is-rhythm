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
#include "structures/screen/pausing.h"

namespace Structures::Screen::Gameplay
{
	using namespace Render::Object;

	struct GameplayScreen final
	{
	protected:
		std::unique_ptr<const Game::Beatmap::Mapset> mapset;
		float mod_multiplier = 1.0f;
		bool no_fail = false;
		bool auto_play = false;
		Events::Event::Buffer event_buffer;
		Events::Action::Buffer action_buffer;
		Pausing::PauseScreen pause_screen;

	public:
		//! Logic
		struct Logic final
		{
		protected:
			GameplayScreen* gameplay_screen;
			bool is_started = false;

		private:
			void sync_audio(const int64_t& current_time) const;
			void update_event(const int64_t& current_time, const Events::Event::Input::SdlEvents& events);
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
				const Game::Beatmap::Hitsound::SampleSet* beatmap_sample; // tự update qua time_step

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


			[[nodiscard]] const bool& is_paused() const;
			void pause();
			void resume();
			void fail(const int64_t& current_time) const;
			void retry();
			void make_time_step(const Events::Event::Input::SdlEvents& events);

			explicit Logic(
				GameplayScreen* gameplay_screen,
				const float* mod_multiplier,
				const bool* no_fail);
		} logic;

		//! Core
		struct Render final
		{
			struct
			{
				Structures::Render::Layer::Layer::Buffer::Item cursor, health, mapset, score;
			} item;

			const Memory* skin;
			bool load_storyboard;
			GameplayScreen* gameplay_screen;
			std::shared_ptr<Cursor::Render::Cursor> cursor;
			std::shared_ptr<Health::Render::Health> health;
			std::shared_ptr<Mapset::Render::Mapset> mapset;
			std::shared_ptr<Score::Render::Score> score;
			std::unique_ptr<Game::Beatmap::Event::EventObjects> storyboard;

			void clean(bool exit = false);
			void retry();

			explicit Render(
				GameplayScreen* gameplay_screen,
				const Memory& skin, bool load_storyboard);
			~Render();
		} render;

		//! Audio
		struct Audio final
		{
			Structures::Audio::MusicMemory::Item audio_file;
			GameplayScreen* gameplay_screen;
			Structures::Audio::MusicMemory* beatmap_music;
			Structures::Audio::EffectMemory* beatmap_effect;
			Structures::Audio::EffectMemory* skin_effect;


			void check_and_play_sound(
				const int64_t& current_time,
				const Game::Beatmap::HitObjects::Floor& floor,
				const Types::Game::Gameplay::NoteScore& score,
				const Game::Beatmap::Hitsound::TimingSample& timing_sample, 
				const Game::Beatmap::Hitsound::SampleSet& beatmap_sample) const;
			void clean(bool exit = false) const;
			void retry() const;

			explicit Audio(GameplayScreen* gameplay_screen,
				const std::filesystem::path& mapset_root,
				Structures::Audio::MusicMemory* beatmap_music,
				Structures::Audio::EffectMemory* beatmap_effect,
				Structures::Audio::EffectMemory* skin_effect);
			~Audio();
		} audio;

		void retry();

		explicit GameplayScreen(
			const std::filesystem::path& mapset_path, 
			const float& mod_multiplier, bool load_storyboard, 
			bool no_fail = false, bool auto_play = false);
		~GameplayScreen();
	};
}