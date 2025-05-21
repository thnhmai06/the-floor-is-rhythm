#include "structures/screen/gameplay.h" // Header
#include <ranges>
#include "format/skin.h"
#include "structures/screen/gameplay/mapset.h"
#include "structures/events/event/internal/gameplay.h"
#include "structures/events/condition/external/pressed.h"

namespace Structures::Screen::Gameplay
{
	//! Events
	GameplayScreen::Events::Events(const Engine::Events::Event::External::Buffer& external_events)
		: external(&external_events)
	{
	}

	//! Actions
	void GameplayScreen::Actions::execute(const bool only_logic)
	{
		logic.execute();
		if (!only_logic)
		{
			gameplay.execute();
			storyboard.execute();
		}
	}
	void GameplayScreen::Actions::clear()
	{
		gameplay.data.clear();
		storyboard.data.clear();
	}
	GameplayScreen::Actions::Actions(
		const std::weak_ptr<const Engine::Events::Timing::Timer>& timer)
		: gameplay(timer), storyboard(timer)
	{
	}

	//! System
	// ::
	bool GameplayScreen::System::update_pause() const
	{
		using Structures::Events::Condition::External::External::KeyDownCondition;

		const auto& current_time = screen->timer->get_last_point();
		const auto& external = *screen->events.external;
		const auto pause_condition = std::make_shared<KeyDownCondition>(
			std::unordered_set{ ::Config::Game::KeyBinding::Pausing::KEY_PAUSE }, current_time);

		return external.contains(pause_condition);
	}
	void GameplayScreen::System::update_input()
	{
		const auto& current_time = screen->timer->get_last_point();
		if (screen->auto_play)
		{
			const auto& object = screen->current.hit_object->second;

			key_stroke.reset(true);
			if (object.time <= current_time)
			{
				if (object.is_kat)
					key_stroke.r1.make_virtual_event(true);
				else key_stroke.l1.make_virtual_event(true);
			}
		}
		else key_stroke.update();
	}
	bool GameplayScreen::System::update_gameplay()
	{
		using Core::Type::Game::Gameplay::NoteScore;
		using namespace Structures::Events::Event::Internal::Gameplay;

		const auto& mapset = *screen->mapset;
		const auto& current_time = screen->timer->get_last_point();
		auto click_left = key_stroke.get_recently_pressed_left();
		auto click_right = key_stroke.get_recently_pressed_right();
		auto& [hit_object, timing_point] = screen->current;

		while (timing_point != mapset.timing_points.data.end()
			&& timing_point->first <= current_time)
			++timing_point;
		if (timing_point == mapset.timing_points.data.end())
			timing_point = Utilities::Container::get_last_element_iterator(mapset.timing_points.data);
		bool is_alive = true;
		while (hit_object != mapset.hit_objects.data.end() && is_alive)
		{
			const auto& floor = hit_object->second;
			auto note_score = NoteScore::Skip;
			if (floor.is_kat)
				note_score = score.get_floor_score(floor, click_left, current_time);
			else note_score = score.get_floor_score(floor, click_right, current_time);
			if (note_score == NoteScore::Skip) break; // chưa đến
			screen->events.internal.submit(std::make_shared<Scoring>(current_time, hit_object, note_score));

			const auto previous_combo = *score.combo.get_current_combo();
			score.update(note_score);
			if (*score.combo.get_current_combo() == 0)
				screen->events.internal.submit(
					std::make_shared<ComboBreak>(current_time, previous_combo));
			is_alive = screen->no_fail || screen->system.health.update(note_score, previous_combo);
		}
		return is_alive;
	}
	void GameplayScreen::System::update()
	{
		if (update_pause())
		{
			if (!screen->paused)
				screen->pause(false);
			else screen->resume();
		}
		if (screen->paused) return;
		if (screen->current.hit_object != screen->mapset->hit_objects.data.end())
			update_input();
		if (!update_gameplay())
			screen->pause(true);
	}
	/*void GameplayScreen::System::check_finish(const int64_t& current_time)
	{
		const auto& mapset = screen->mapset;
		const auto current_music_time = current_time - mapset->general.start_music_delay;

		if (current_music_time >= mapset->stats.time.start_playing_time + mapset->stats.time.length)
			is_finished = true;
	}*/
	void GameplayScreen::System::new_play()
	{
		key_stroke.reset();
		score.reset();
		health.reset();
	}
	GameplayScreen::System::System(GameplayScreen& screen)
		: screen(&screen), key_stroke(*screen.events.external, screen.timer),
		score(*screen.mapset, &screen.score_multiplier),
		health(*screen.mapset, &screen.timer->is_paused(), &screen.no_fail)
	{
	}

	//! Render
	// ::Layers
	void GameplayScreen::Render::Layers::render() const
	{
		background->render();
		storyboard->render();
		playground->render();
		hud->render();
		sticky_hud->render();
		foreground->render();
	}
	GameplayScreen::Render::Layers::Layers()
	{
		background = std::make_unique<Engine::Render::Layer::Layer>();
		storyboard = std::make_unique<Layer::Storyboard>(renderer);
		playground = std::make_unique<Engine::Render::Layer::Layer>();
		hud = std::make_unique<Engine::Render::Layer::Layer>();
		sticky_hud = std::make_unique<Engine::Render::Layer::Layer>();
		foreground = std::make_unique<Engine::Render::Layer::Layer>();
	}
	// ::
	void GameplayScreen::Render::new_play()
	{
		// Xóa
		layers.storyboard->clear(); objects.storyboard.reset();
		items.cursor.destroy(); objects.cursor.reset(); // Cần vẽ lại cursor vì còn note score trên đó

		// Tạo
		objects.cursor = std::make_shared<Cursor::Render::Cursor>(*skin.lock(), screen->timer);
		items.cursor = layers.sticky_hud->buffer.add(objects.cursor);
		if (load_storyboard)
		{
			const auto& beatmap_root = screen->mapset->path.parent_path();
			layers.storyboard->resize(screen->mapset->general.widescreen_storyboard);
			objects.storyboard = std::make_unique<Game::Beatmap::Event::EventObjects>(
				screen->mapset->events, renderer,
				beatmap_root, screen->timer,
				screen->actions.storyboard, screen->events.internal);
			objects.storyboard->submit_to_buffer(
				layers.background->buffer,
				layers.storyboard->layer.buffer);
		}
	}
	void GameplayScreen::Render::update() const
	{
		const auto& mapset = *screen->mapset;
		const auto current_time = screen->timer->get_last_point();
		const auto scoring_condition =
			std::make_shared<Engine::Events::Condition::Internal::Condition>(
				std::set{ Core::Type::Event::Scoring }, current_time);

		const auto& search_res = screen->events.internal.search(scoring_condition);
		if (search_res.contains(Core::Type::Event::Scoring))
		{
			const auto& scoring_events = search_res.at(Core::Type::Event::Scoring);
			for (const auto& event : scoring_events | std::views::values)
			{
				if (const auto ptr = std::dynamic_pointer_cast<const Structures::Events::Event::Internal::Gameplay::Scoring>(event.lock()))
					objects.mapset->get_scripts().at(&ptr->object->second).lock()->config.visible = false;
			}
			objects.cursor->check_and_add_score_shown(scoring_events, screen->actions.gameplay);
		}
		objects.progress->percent =
			Utilities::Math::to_percent(current_time, mapset.stats.time.start_time,
				mapset.stats.time.start_playing_time + mapset.stats.time.length);
		screen->actions.execute();
		layers.render();
	}
	void GameplayScreen::Render::pause(const bool fail) const
	{
		objects.pausing->pause(fail);
	}
	void GameplayScreen::Render::resume() const
	{
		objects.pausing->resume();
	}
	GameplayScreen::Render::Render(GameplayScreen* screen,
		std::weak_ptr<const Engine::Render::Texture::Memory> skin, const bool load_storyboard)
		: screen(screen), skin(std::move(skin)), load_storyboard(load_storyboard)
	{
		if (const auto& locked_skin = Utilities::Pointer::check_weak(this->skin))
		{
			// init
			objects.mapset = std::make_shared<Mapset::Render::Mapset>(*locked_skin, screen->actions.gameplay, *screen->mapset);
			objects.health = std::make_shared<Health::Render::Health>(*locked_skin, screen->system.health.get_health(), screen->timer);
			objects.progress = std::make_shared<Object::StaticPercentObject>(
				locked_skin->find(Format::Skin::Image::progress),
				::Config::Game::Render::Progress::get_full_size(), true, false,
				::Config::Game::Render::Progress::ORIGIN, ::Config::Game::Render::Progress::get_pos());
			objects.score = std::make_shared<Score::Render::Score>(*locked_skin,
				screen->system.score.get_score(),
				screen->system.score.accuracy.get_accuracy(),
				screen->system.score.combo.get_current_combo());
			objects.pausing = std::make_unique<Pausing::Pausing>(
				*locked_skin, *screen->events.external, screen->actions.logic,
				[this] { this->screen->resume(); }, [this] { this->screen->new_play(); }, [] { is_running = false; });

			// submit
			items.mapset = layers.playground->buffer.add(objects.mapset);
			items.health = layers.hud->buffer.add(objects.health);
			items.score = layers.hud->buffer.add(objects.score);
			items.progress = layers.hud->buffer.add(objects.progress);
			items.pausing = layers.foreground->buffer.add(objects.pausing);
		}

		new_play();
	}

	//! Audio
	void GameplayScreen::Audio::play_hit_sound(
		const Engine::Events::Timing::Time& time,
		const Game::Beatmap::HitObjects::Floor& floor,
		const Game::Beatmap::Hitsound::TimingSample& timing_sample,
		const Game::Beatmap::Hitsound::SampleSet& mapset_sample) const
	{
		const auto sound_names =
			Game::Beatmap::Hitsound::get_hit_sound_filename(
				floor.additions, floor.hit_sample, timing_sample, mapset_sample, *effect.beatmap);
		for (const auto& sound_name : sound_names)
		{
			auto sound_file = effect.beatmap->find(sound_name, true); // beatmap
			if (!sound_file.is_valid())
			{
				if (const auto& effect_skin = Utilities::Pointer::check_weak(effect.skin))
					sound_file = effect_skin->find(sound_name); // skin
			}
			mixer->effect.play(sound_file, Game::Beatmap::Hitsound::get_hit_sample_volume(floor.hit_sample, timing_sample));
		}
		auto hit_sound_event = std::make_shared<Structures::Events::Event::Internal::Gameplay::Hitsound>(
			time, floor, timing_sample, mapset_sample, *effect.beatmap);
		screen->events.internal.submit(std::move(hit_sound_event));
	}
	void GameplayScreen::Audio::sync_audio() const
	{
		if (screen->timer->is_paused() || !Engine::Audio::Bus::Bus<Engine::Audio::Music>::has_song_playing()) return;

		const auto current_time = screen->timer->get_last_point();
		const auto& mapset = screen->mapset;
		const auto current_music_time = mixer->music.get_position();
		if (const auto needed_music_time = current_time - mapset->general.start_music_delay;
			needed_music_time >= 0)
		{
			if (!Utilities::Math::in_offset_range(needed_music_time, 50LL, current_music_time))
				Engine::Audio::Bus::Bus<Engine::Audio::Music>::seek(needed_music_time);
			if (!Engine::Audio::Bus::Bus<Engine::Audio::Music>::is_playing())
				Engine::Audio::Bus::Bus<Engine::Audio::Music>::resume();
		}
	}
	void GameplayScreen::Audio::new_play() const
	{
		mixer->stop_all();
		mixer->music.play(music.audio_file, true);
	}
	void GameplayScreen::Audio::update() const
	{
		const auto current_time = screen->timer->get_last_point();
		const auto scoring_condition =
			std::make_shared<Engine::Events::Condition::Internal::Condition>(
				std::set{ Core::Type::Event::Scoring }, current_time);

		for (const auto scoring_events
			= screen->events.internal.search(scoring_condition)[Core::Type::Event::Scoring];
			const auto& event : scoring_events | std::views::values)
		{
			if (const auto ptr = std::dynamic_pointer_cast<const Structures::Events::Event::Internal::Gameplay::Scoring>(event.lock()))
			{
				if (ptr->score != Core::Type::Game::Gameplay::NoteScore::Miss
					&& ptr->score != Core::Type::Game::Gameplay::NoteScore::Skip)
				{
					play_hit_sound(current_time, ptr->object->second,
						screen->current.timing_point->second.sample, screen->mapset->general.sample_set);
				}
			}
		}
		sync_audio();
	}
	void GameplayScreen::Audio::pause(const bool fail) const
	{
		Engine::Audio::Bus::Bus<Engine::Audio::Music>::pause();
		if (fail)
		{
			const auto fail_sound = effect.skin.lock()->find(Format::Skin::Sound::fail_sound);
			mixer->effect.play(fail_sound);
		}
	}
	void GameplayScreen::Audio::resume() const
	{
		const auto current_time = screen->timer->get_last_point();
		const auto& mapset = screen->mapset;
		const auto current_music_time = current_time - mapset->general.start_music_delay;

		if (current_music_time >= 0 && Engine::Audio::Bus::Bus<Engine::Audio::Music>::has_song_playing())
			Engine::Audio::Bus::Bus<Engine::Audio::Music>::resume();
	}
	GameplayScreen::Audio::Audio(
		GameplayScreen* gameplay_screen,
		const std::filesystem::path& mapset_root,
		std::weak_ptr<const Engine::Audio::Memory::EffectMemory> effect_skin)
		: screen(gameplay_screen)
	{
		effect.skin = std::move(effect_skin);
		effect.beatmap = std::make_unique<Engine::Audio::Memory::EffectMemory>();
		music.beatmap = std::make_unique<Engine::Audio::Memory::MusicMemory>();

		const auto audio_path = mapset_root / gameplay_screen->mapset->general.audio_file;
		music.audio_file = music.beatmap->load(audio_path, audio_path.filename().string());
		effect.beatmap->load(mapset_root, mapset_root, false,
			false, false, false, { audio_path.stem() });

		new_play();
	}

	//! Screen
	void GameplayScreen::pause(const bool fail)
	{
		if (paused || (pause_lock && !fail)) return;
		audio.pause(fail);
		render.pause(fail);
		timer->pause();

		paused = true;
		if (fail) no_resume = true;
	}
	void GameplayScreen::resume()
	{
		if (!paused || no_resume) return;
		audio.resume();
		render.resume();
		timer->resume();

		const auto& current_tick = SDL_GetTicks();
		paused = false; pause_lock = true;
		actions.gameplay.submit(std::make_shared<Engine::Events::Action::CallbackAction>(
			current_tick + ::Config::Game::General::PAUSE_LOCK, [this](const int64_t&) { this->pause_lock = false; }));
	}
	void GameplayScreen::new_play()
	{
		current.hit_object = mapset->hit_objects.data.begin();
		current.timing_point = mapset->timing_points.data.begin();
		events.internal.clear();
		actions.clear();

		timer->reset(false);
		audio.new_play();
		render.new_play();
		system.new_play();
	}
	void GameplayScreen::update(Engine::Screen::Stack& screens_stack)
	{
		const auto current_time = timer->make_point();
		if (!started)
		{
			resume();
			started = true;
		}
		events.internal.clear_old(current_time);

		system.update();
		audio.update();
		render.update(); // đã bao gồm actions trong đây
	}
	GameplayScreen::GameplayScreen(
		const Engine::Events::Event::External::Buffer& external_events,
		const std::filesystem::path& mapset_path,
		std::weak_ptr<const Engine::Render::Texture::Memory> skin_image,
		std::weak_ptr<const Engine::Audio::Memory::EffectMemory> skin_effect,
		const float& score_multiplier, const bool load_storyboard,
		const bool no_fail, const bool auto_play)
		: score_multiplier(score_multiplier), no_fail(no_fail), auto_play(auto_play),
		mapset(std::make_unique<const Game::Beatmap::Mapset>(mapset_path, load_storyboard)),
		timer(std::make_shared<Engine::Events::Timing::Timer>(mapset->stats.time.start_time, false)),
		events(external_events), actions(this->timer), system(*this),
		render(this, std::move(skin_image), load_storyboard),
		audio(this, mapset_path.parent_path(), std::move(skin_effect))
	{
		new_play();
	}
}