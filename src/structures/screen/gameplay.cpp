// ReSharper disable CppClangTidyClangDiagnosticShadow
#include "structures/screen/gameplay.h" // Header
#include "main.h"
#include "structures/screen/gameplay/mapset.h"
#include "structures/events/event/playing.h"
#include "core/resources/layer.h"
#include "core/resources/textures.h"
#include "logging/logger.h"
#include "core/resources/audio.h"

namespace Structures::Screen::Gameplay
{
	//! Logic
	const bool& GameplayScreen::Logic::is_paused() const { return system.timer.is_paused(); }
	void GameplayScreen::Logic::sync_audio(const int64_t& current_time) const
	{
		if (is_paused()) return;
		const auto& mapset = gameplay_screen->mapset;

		const auto current_pos = mixer->music.get_position();
		const auto current_music_time = current_time - mapset->general.start_music_delay;
		if (current_music_time >= 0)
		{
			if (!Utilities::Math::in_offset_range(current_music_time, 500LL, current_pos))
				Structures::Audio::Bus<Types::Audio::Music>::seek(current_music_time);
			if (!Structures::Audio::Bus<Types::Audio::Music>::is_playing())
				Structures::Audio::Bus<Types::Audio::Music>::resume();
		}
	}
	void GameplayScreen::Logic::update_event(const int64_t& current_time, const Events::Event::Input::SdlEvents& events)
	{
		// Chung
		Events::Event::Input::KeyboardEvents keyboard_events{};
		Events::Event::Input::MouseEvents mouse_events{};
		for (const auto& event : events)
		{
			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) mouse_events.push_back(event.button); // chỉ cần thế này là đủ (hiện tại)
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == ::Config::Game::KeyBinding::Pausing::KEY_PAUSE)
			{
				gameplay_screen->pause_screen.toggle_pause();
				return;
			}
		}
		gameplay_screen->pause_screen.check_event(mouse_events);

		// Input chơi game
		if (is_paused()) return;
		if (gameplay_screen->auto_play)
		{
			system.key_stroke.reset(true);
			if (const auto& r_scripts = *gameplay_screen->render.mapset->get_render_scripts();
				current.object_script != r_scripts.end())
			{
				if (const auto& [time, object] = *current.object_script;
					time <= current_time)
				{
					if (object.is_kat) system.key_stroke.r1.make_virtual_event(true);
					else system.key_stroke.l1.make_virtual_event(true);
				}
			}
		}
		else
		{
			for (const auto& event : events)
			{
				if (Events::Event::Input::key_event_types.contains(event.type))
					keyboard_events.push_back(event.key);
			}
			system.key_stroke.update(keyboard_events);
		}
	}
	void GameplayScreen::Logic::update_timing(const int64_t& current_time)
	{
		if (is_paused()) return;
		const auto& timing_points = gameplay_screen->mapset->timing_points.data;
		const auto& crr_timing_point = timing_points.lower_bound(current_time);
		if (crr_timing_point == timing_points.end()) return;
		current.timing_sample = crr_timing_point->second.sample;
	}
	bool GameplayScreen::Logic::update_score_and_health(const Types::Game::Gameplay::NoteScore& obj_score)
	{
		if (obj_score != Types::Game::Gameplay::NoteScore::Skip)
		{
			const auto previous_combo = *system.score.combo.get_current_combo();
			system.score.update(obj_score);
			if (*system.score.combo.get_current_combo() == 0 && previous_combo >= 10)
				mixer->effect.play(gameplay_screen->audio.skin_effect->find(Format::Skin::Sound::combo_break));
			return system.health.update(obj_score, previous_combo);
		}
		return true;
	}
	bool GameplayScreen::Logic::update_object(const int64_t& current_time, uint16_t& click_left, uint16_t& click_right)
	{
		const auto& r_objects_script = *gameplay_screen->render.mapset->get_render_scripts();
		bool is_alive = true;
		while (current.object_script != r_objects_script.end() && is_alive)
		{
			//? Tính điểm
			const auto& r_object = dynamic_cast<Mapset::Render::Components::Floor*>(current.object_script->second.r_obj.lock().get());
			const auto& floor = *r_object->object;
			auto floor_score = Types::Game::Gameplay::NoteScore::Skip;
			if (floor.is_kat)
				floor_score = system.score.get_floor_score(floor, click_right, current_time);
			else
				floor_score = system.score.get_floor_score(floor, click_left, current_time);
			if (floor_score == Types::Game::Gameplay::NoteScore::Skip) break; // chưa đến

			//? Cập nhật
			is_alive = update_score_and_health(floor_score);
			r_object->visible = false;
			gameplay_screen->event_buffer.add(current_time, std::make_shared<Events::Event::Playing::Scoring>(&floor, floor_score));
			gameplay_screen->audio.check_and_play_sound(current_time, floor, floor_score, current.timing_sample, *current.beatmap_sample);
			++current.object_script;
		}
		return is_alive;
	}
	void GameplayScreen::Logic::update_render(const int64_t& current_time) const
	{
		const auto& mapset = gameplay_screen->mapset;

		gameplay_screen->render.mapset->set_current_pos(current_time);
		gameplay_screen->render.mapset->set_render_range(current_time);
		gameplay_screen->render.progress->percent = 
			Utilities::Math::to_percent(current_time,
				mapset->stats.time.start_time,
				mapset->stats.time.start_playing_time + mapset->stats.time.length);
	}
	void GameplayScreen::Logic::update_event_and_action(const int64_t& current_time) const
	{
		//! Event
		gameplay_screen->render.cursor->check_and_add_score_shown(gameplay_screen->event_buffer, gameplay_screen->gameplay_buffer, current_time);
		// event Hitsound đã được thực thi ở hàm Audio::check_and_play_sound (khi đang update_object)

		//! Action
		gameplay_screen->storyboard_buffer.execute(current_time);
		gameplay_screen->gameplay_buffer.execute(current_time);
	}
	void GameplayScreen::Logic::check_finish(const int64_t& current_time)
	{
		const auto& mapset = gameplay_screen->mapset;
		const auto current_music_time = current_time - mapset->general.start_music_delay;

		if (current_music_time >= mapset->stats.time.start_playing_time + mapset->stats.time.length)
			is_finished = true;
	}
	void GameplayScreen::Logic::pause()
	{
		Structures::Audio::Bus<Types::Audio::Music>::pause();
		system.timer.pause();
	}
	void GameplayScreen::Logic::resume()
	{
		const auto current_time = system.timer.get_time();
		const auto& mapset = gameplay_screen->mapset;
		const auto current_music_time = current_time - mapset->general.start_music_delay;

		if (current_music_time >= 0 && Structures::Audio::Bus<Types::Audio::Music>::has_song_playing())
			Structures::Audio::Bus<Types::Audio::Music>::resume();
		system.timer.resume();
	}
	void GameplayScreen::Logic::fail(const int64_t& current_time) const
	{
		gameplay_screen->pause_screen.make_fail();

		const auto fail_sound = gameplay_screen->audio.skin_effect->find(Format::Skin::Sound::fail_sound);
		mixer->effect.play(fail_sound);
	}
	void GameplayScreen::Logic::make_time_step(const Events::Event::Input::SdlEvents& events)
	{
		// bắt đầu timer (nếu chưa start)
		if (!is_started)
		{
			is_started = true;
			resume();
		}
		gameplay_screen->event_buffer.clear();
		const auto current_time = system.timer.get_time();

		sync_audio(current_time);
		update_event(current_time, events);
		if (!is_started) return;
		if (!is_paused() && !is_finished)
		{
			update_timing(current_time);

			auto click_left = system.key_stroke.get_recently_pressed_left();
			auto click_right = system.key_stroke.get_recently_pressed_right();
			if (!update_object(current_time, click_left, click_right))
				fail(current_time);
		}
		//if (!is_finished)
		//{
		//	//check_finish(current_time);
		//	is_finished = true;
		//	if (is_finished) gameplay_screen->render.show_result(current_time);
		//}
		update_render(current_time);
		update_event_and_action(current_time);
	}
	void GameplayScreen::Logic::retry()
	{
		// Current
		const auto& mapset = gameplay_screen->mapset;
		const auto& r_mapset = gameplay_screen->render.mapset;
		for (auto& direction : r_mapset->data)
		{
			if (std::holds_alternative<std::shared_ptr<Collection>>(direction))
			{
				auto& it = std::get<std::shared_ptr<Collection>>(direction);
				for (auto& object : it->data)
				{
					if (std::holds_alternative<std::shared_ptr<Collection>>(object))
					{
						auto& floor = std::get<std::shared_ptr<Collection>>(object);
						floor->visible = true;
					}
				}
			}
		}
		current.object_script = r_mapset->get_render_scripts()->begin();

		// System
		system.timer.reset(false, mapset->stats.time.start_time);
		system.key_stroke.reset();
		system.score.reset();
		system.health.reset();

		// out
		is_started = is_finished = false;
		gameplay_screen->pause_screen.clean();
	}
	GameplayScreen::Logic::Logic(
		GameplayScreen* gameplay_screen,
		const float* mod_multiplier,
		const bool* no_fail)
		: gameplay_screen(gameplay_screen),
		current(*gameplay_screen->mapset), system(*gameplay_screen->mapset, no_fail, mod_multiplier)
	{
	}
	GameplayScreen::Logic::Current::Current(const Game::Beatmap::Mapset& mapset)
		: beatmap_sample(&mapset.general.sample_set)
	{
	}
	GameplayScreen::Logic::System::System(
		const Game::Beatmap::Mapset& mapset,
		const bool* no_fail, const float* mod_multiplier)
		: score(mapset, mod_multiplier), health(mapset, &timer.is_paused(), no_fail)
	{
	}

	//! Render
	void GameplayScreen::Render::clean(const bool exit)
	{
		Core::Resources::Layers::normal_background->clear();
		Core::Resources::Layers::storyboard->clear();
		storyboard.reset();

		// Cần vẽ lại cursor vì còn note score
		item.cursor.destroy();
		cursor.reset();

		if (exit)
		{
			item.mapset.destroy();
			item.health.destroy();
			item.score.destroy();
			item.progress.destroy();

			mapset.reset();
			health.reset();
			score.reset();
			progress.reset();
		}
	}
	void GameplayScreen::Render::retry()
	{
		clean();

		cursor = std::make_shared<Cursor::Render::Cursor>(*skin);
		item.cursor = Core::Resources::Layers::static_hud->render_buffer.add(this->cursor);

		if (load_storyboard)
		{
			Core::Resources::Layers::storyboard->resize(gameplay_screen->mapset->general.widescreen_storyboard);
			storyboard = std::make_unique<Game::Beatmap::Event::EventObjects>(
				gameplay_screen->mapset->events, renderer,
				gameplay_screen->mapset->path.parent_path(), &gameplay_screen->logic.system.timer,
				gameplay_screen->storyboard_buffer, &gameplay_screen->event_buffer);
			storyboard->submit_to_buffer(
				Core::Resources::Layers::normal_background.get(), &Core::Resources::Layers::storyboard->background,
				&Core::Resources::Layers::storyboard->fail, &Core::Resources::Layers::storyboard->pass,
				&Core::Resources::Layers::storyboard->foreground);
		}
	}
	void GameplayScreen::Render::show_result(const int64_t& current_time)
	{
		using namespace ::Config::Game::Render::Result;
		result = std::make_shared<Result::Result>(*skin, gameplay_screen->logic.system.score, *score);
		item.result = Core::Resources::Layers::foreground->render_buffer.add(result);

		Core::Resources::Layers::storyboard->visible = false;
		Core::Resources::Layers::hud->visible = false;
		Core::Resources::Layers::static_hud->visible = false;
		Core::Resources::Layers::playground->visible = false;

		auto& [data] = gameplay_screen->storyboard_buffer;
		data.emplace(current_time + DELAY, std::make_shared<Events::Action::Render::ChangeAction<float>>(
			current_time + DELAY, current_time + DELAY + TIME_MOVE_IN_RESULT,
			EASING_IN_RESULT, &result->offset.x, result->offset.x, 0 ));
	}
	GameplayScreen::Render::Render(
		GameplayScreen* gameplay_screen,
		const Memory& skin, const bool load_storyboard)
		: skin(&skin), load_storyboard(load_storyboard), gameplay_screen(gameplay_screen)
	{
		// Setup
		mapset = std::make_shared<Mapset::Render::Mapset>(skin, *gameplay_screen->mapset);
		health = std::make_shared<Health::Render::Health>(skin, gameplay_screen->logic.system.health.get_health());
		progress = std::make_shared<StaticPercentObject>(skin.find(Format::Skin::Image::progress),
			::Config::Game::Render::Progress::get_full_size(), true, false,
			::Config::Game::Render::Progress::ORIGIN, ::Config::Game::Render::Progress::get_pos());
		score = std::make_shared<Score::Render::Score>(skin, gameplay_screen->logic.system.score.get_score(),
			gameplay_screen->logic.system.score.accuracy.get_accuracy(),
			gameplay_screen->logic.system.score.combo.get_current_combo());

		// Submit lên on_before_render buffer của layers
		item.mapset = Core::Resources::Layers::playground->render_buffer.add(this->mapset);
		item.health = Core::Resources::Layers::hud->render_buffer.add(this->health);
		item.score = Core::Resources::Layers::hud->render_buffer.add(this->score);
		item.progress = Core::Resources::Layers::hud->render_buffer.add(this->progress);

		retry();
	}
	GameplayScreen::Render::~Render() { clean(true); }

	//! Audio
	void GameplayScreen::Audio::check_and_play_sound(
		const int64_t& current_time,
		const Game::Beatmap::HitObjects::Floor& floor,
		const Types::Game::Gameplay::NoteScore& score,
		const Game::Beatmap::Hitsound::TimingSample& timing_sample, const Game::Beatmap::Hitsound::SampleSet& beatmap_sample) const
	{
		if (score == Types::Game::Gameplay::NoteScore::Skip
			|| score == Types::Game::Gameplay::NoteScore::Miss) return;

		const auto hit_sound_files =
			Game::Beatmap::Hitsound::get_hit_sound_filename(floor.hit_sound, floor.hit_sample, timing_sample, beatmap_sample, *beatmap_effect);
		for (const auto& sound_name : hit_sound_files)
		{
			auto sound = beatmap_effect->find(sound_name, true); // beatmap
			if (!sound.is_valid()) sound = skin_effect->find(sound_name); // skin
			mixer->effect.play(sound, Game::Beatmap::Hitsound::get_hit_sample_volume(floor.hit_sample, timing_sample));
			auto hit_sound_event = std::make_shared<Events::Event::Playing::Hitsound>(floor, *gameplay_screen->mapset, *beatmap_effect);
			gameplay_screen->event_buffer.add(current_time, std::move(hit_sound_event));
		}
	}
	void GameplayScreen::Audio::clean(const bool exit) const
	{
		mixer->music.stop();
		Structures::Audio::Bus<Mix_Chunk*>::stop();
		if (exit)
		{
			beatmap_music->free_all();
			beatmap_effect->free_all();
			skin_effect->free_all();
		}
	}
	void GameplayScreen::Audio::retry() const
	{
		clean();
		mixer->music.play(audio_file, true);
	}

	GameplayScreen::Audio::Audio(
		GameplayScreen* gameplay_screen,
		const std::filesystem::path& mapset_root,
		Structures::Audio::MusicMemory* beatmap_music,
		Structures::Audio::EffectMemory* beatmap_effect,
		Structures::Audio::EffectMemory* skin_effect)
		: gameplay_screen(gameplay_screen), beatmap_music(beatmap_music), beatmap_effect(beatmap_effect), skin_effect(skin_effect)
	{
		clean();

		// load audio
		const auto beatmap_audio_file = mapset_root / gameplay_screen->mapset->general.audio_file;
		audio_file = beatmap_music->load(beatmap_audio_file, beatmap_audio_file.filename().string());
		beatmap_effect->load(mapset_root, mapset_root, false, false,
			false, false, { beatmap_audio_file.stem() });

		mixer->music.play(audio_file, true);
	}
	GameplayScreen::Audio::~Audio() { clean(true); }

	//! Screen
	void GameplayScreen::retry()
	{
		event_buffer.clear();
		storyboard_buffer.data.clear();

		audio.retry();
		render.retry();
		logic.retry();
	}
	GameplayScreen::GameplayScreen(
		const std::filesystem::path& mapset_path,
		const float& mod_multiplier,
		const bool load_storyboard,
		const bool no_fail, const bool auto_play)
		: mapset(std::make_unique<const Game::Beatmap::Mapset>(mapset_path, load_storyboard)),
		mod_multiplier(mod_multiplier), no_fail(no_fail), auto_play(auto_play),
		pause_screen(*Core::Resources::Textures::skin, Core::Resources::Layers::foreground.get(),
			[&] { logic.pause(); }, [&] { logic.resume(); },
			[&] { pause_screen.make_unpause(); },
			[&] { retry(); }, [] { is_running = false; /*Tạm vậy*/ }),
		logic(this, &this->mod_multiplier, &this->no_fail),
		render(this, *Core::Resources::Textures::skin, load_storyboard),
		audio(this, mapset_path.parent_path(),
			&Core::Resources::Audio::Beatmap::music,
			&Core::Resources::Audio::Beatmap::effect,
			&Core::Resources::Audio::Skin::effect)
	{
		logic.retry();
	}
	GameplayScreen::~GameplayScreen() { mapset.reset(); }
}