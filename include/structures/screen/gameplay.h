// ♪ https://youtu.be/i0fw1thgnK0 <3
#pragma once
#include "engine/screen/screen.h"
#include "structures/render/layer/storyboard.h"
#include "structures/screen/gameplay/keystroke.h"
#include "structures/screen/gameplay/cursor.h"
#include "structures/screen/gameplay/health.h"
#include "structures/screen/gameplay/score.h"
#include "structures/screen/gameplay/mapset.h"
#include "structures/screen/gameplay/pausing.h"
#include "structures/screen/gameplay/result.h"

namespace Structures::Screen::Gameplay
{
	using namespace Structures::Render;

	struct GameplayScreen : Engine::Screen::Screen
	{
	protected:
		bool started = false;
		bool paused = false; bool no_resume = false; bool pause_lock = false;
		bool playing = false;

	public:
		float score_multiplier; bool no_fail; bool auto_play;
		std::unique_ptr<const Game::Beatmap::Mapset> mapset;
		std::shared_ptr<Engine::Events::Timing::Timer> timer;
		struct
		{
			Game::Beatmap::HitObjects::HitObjects::Container::const_iterator hit_object;
			Game::Beatmap::TimingPoints::TimingPoints::Container::const_iterator timing_point;
		} current;
		struct Events
		{
			const Engine::Events::Event::External::Buffer* external;
			Engine::Events::Event::Internal::Buffer internal;

			explicit Events(const Engine::Events::Event::External::Buffer& external_events);
		} events;
		struct Actions
		{
			Engine::Events::Action::Buffer logic;
			Engine::Events::Action::Buffer gameplay;
			Engine::Events::Action::Buffer storyboard;

			void execute(bool only_logic = false);
			void clear();

			explicit Actions(
				const std::weak_ptr<const Engine::Events::Timing::Timer>& timer);
		} actions;
		struct System
		{
			GameplayScreen* screen;

			KeyStroke::Logic::Keystroke key_stroke;
			Score::Logic::Score score;
			Health::Logic::Health health;

		protected:
			bool update_pause() const; // -> có nhấn nút esc không?
			void update_input();
			bool update_gameplay(); // -> người chơi còn sống không?

		public:
			void update();
			void new_play();

			explicit System(GameplayScreen& screen);
		} system;
		struct Render final
		{
		protected:
			GameplayScreen* screen;
			std::weak_ptr<const Engine::Render::Texture::Memory> skin;
			bool load_storyboard;

		public:
			struct Layers
			{
				std::unique_ptr<Engine::Render::Layer::Layer> background;
				std::unique_ptr<Layer::Storyboard> storyboard;
				std::unique_ptr<Engine::Render::Layer::Layer> playground;
				std::unique_ptr<Engine::Render::Layer::Layer> hud;
				std::unique_ptr<Engine::Render::Layer::Layer> sticky_hud;
				std::unique_ptr<Engine::Render::Layer::Layer> foreground;

				void render() const;
				explicit Layers();
			} layers;
			struct
			{
				Object::Buffer::Item cursor, health, mapset, score, progress;
				Object::Buffer::Item pausing, result;
			} items;
			struct
			{
				std::unique_ptr<Game::Beatmap::Event::EventObjects> storyboard;
				std::shared_ptr<Cursor::Render::Cursor> cursor;
				std::shared_ptr<Health::Render::Health> health;
				std::shared_ptr<Mapset::Render::Mapset> mapset;
				std::shared_ptr<Score::Render::Score> score;
				std::shared_ptr<Object::StaticPercentObject> progress;

				std::shared_ptr<Pausing::Pausing> pausing;
				//std::shared_ptr<Result::Result> result;
			} objects;

			void new_play();
			void pause(bool fail = false) const;
			void resume() const;
			void update() const;

			explicit Render(
				GameplayScreen* screen,
				std::weak_ptr<const Engine::Render::Texture::Memory> skin,
				bool load_storyboard);
		} render;
		struct Audio final
		{
		protected:
			GameplayScreen* screen;

			struct
			{
				std::unique_ptr<Engine::Audio::Memory::MusicMemory> beatmap;
				Engine::Audio::Memory::MusicMemory::Item audio_file;
			} music;

			struct
			{
				std::weak_ptr<const Engine::Audio::Memory::EffectMemory> skin;
				std::unique_ptr<Engine::Audio::Memory::EffectMemory> beatmap;
			} effect;

			void sync_audio() const;
			void play_hit_sound(
				const Engine::Events::Timing::Time& time,
				const Game::Beatmap::HitObjects::Floor& floor,
				const Game::Beatmap::Hitsound::TimingSample& timing_sample,
				const Game::Beatmap::Hitsound::SampleSet& mapset_sample) const;
		public:
			void pause(bool fail = false) const;
			void resume() const;
			void new_play() const;
			void update() const;

			explicit Audio(
				GameplayScreen* gameplay_screen,
				const std::filesystem::path& mapset_root,
				std::weak_ptr<const Engine::Audio::Memory::EffectMemory> effect_skin);
		} audio;

		void pause(bool fail = false);
		void resume();
		void new_play();
		void update(Engine::Screen::Stack& screens_stack) override;

		explicit GameplayScreen(
			const Engine::Events::Event::External::Buffer& external_events,
			const std::filesystem::path& mapset_path,
			std::weak_ptr<const Engine::Render::Texture::Memory> skin_image,
			std::weak_ptr<const Engine::Audio::Memory::EffectMemory> skin_effect,
			const float& score_multiplier = 1.0f, bool load_storyboard = true,
			bool no_fail = false, bool auto_play = false);
	};
}