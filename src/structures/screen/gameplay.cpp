// ReSharper disable CppClangTidyClangDiagnosticShadow
#include "structures/screen/gameplay.h" // Header
#include "structures/screen/gameplay/mapset.h"
#include "structures/events/event/playing.h"
#include "work/render/layer.h"
#include "work/render/textures.h"
#include "logging/logger.h"
#include "work/audio.h"
#include "work/render.h"

namespace Structures::Screen::Gameplay
{
	//! Logic
	const bool* PlayingScreen::Logic::is_paused() const { return system.timer.is_paused(); }
	void PlayingScreen::Logic::pause()
	{
		if (*is_paused()) return;
		Structures::Audio::Bus<Types::Audio::Music>::pause();
		system.timer.pause();
	}
	void PlayingScreen::Logic::resume()
	{
		if (!*is_paused()) return;
		Structures::Audio::Bus<Types::Audio::Music>::resume();
		system.timer.resume();
	}
	void PlayingScreen::Logic::update_input(const int64_t& current_time, const Events::Event::Input::SdlEvents& events)
	{
		// Keystroke
		Events::Event::Input::KeyboardEvents keyboard_events{};
		if (playing_screen->auto_play)
		{
			system.key_stroke.reset(true);
			if (const auto& r_scripts = *playing_screen->render.mapset->get_render_scripts(); 
				current.object_script != r_scripts.end())
			{
				if (const auto& [time, object] = *current.object_script; 
					time <= current_time)
				{
					if (object.is_kat)
						system.key_stroke.r1.make_virtual_event(true);
					else
						system.key_stroke.l1.make_virtual_event(true);
				}
			}
		}
		else
		{
			for (const auto& event : events)
			{
				if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
					keyboard_events.push_back(event.key);
			}
			system.key_stroke.update(keyboard_events);
		}

		playing_screen->event_buffer.clear();
	}
	void PlayingScreen::Logic::update_timing(const int64_t& current_time)
	{
		const auto& timing_points = playing_screen->mapset->timing_points.data;
		const auto& crr_timing_point = timing_points.lower_bound(current_time);
		if (crr_timing_point == timing_points.end()) return;
		current.timing_sample = crr_timing_point->second.sample;
	}
	bool PlayingScreen::Logic::update_score_and_health(const Types::Game::Gameplay::NoteScore& obj_score)
	{
		if (obj_score != Types::Game::Gameplay::NoteScore::Skip)
		{
			system.score.update(obj_score);
			return system.health.update(obj_score, *system.score.combo.get_current_combo());
		}
		return true;
	}
	bool PlayingScreen::Logic::update_object(const int64_t& current_time, uint16_t& click_left, uint16_t& click_right)
	{
		const auto& r_objects_script = *playing_screen->render.mapset->get_render_scripts();
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
			playing_screen->event_buffer.add(current_time, std::make_shared<Events::Event::Playing::Scoring>(&floor, floor_score));
			playing_screen->audio.check_and_play_sound(current_time, floor, floor_score, current.timing_sample, *current.beatmap_sample);
			++current.object_script;
		}
		return is_alive;
	}
	void PlayingScreen::Logic::update_render(const int64_t& current_time) const
	{
		playing_screen->render.mapset->set_current_pos(current_time);
		playing_screen->render.mapset->set_render_range(current_time);
	}
	void PlayingScreen::Logic::update_event_and_action(const int64_t& current_time) const
	{
		//! Event
		playing_screen->render.cursor->check_and_add_score_shown(playing_screen->event_buffer, playing_screen->action_buffer, current_time);
		// event Hitsound đã được thực thi ở hàm Audio::check_and_play_sound

		//! Action
		playing_screen->action_buffer.execute(current_time);
	}

	void PlayingScreen::Logic::make_time_step(const Events::Event::Input::SdlEvents& events)
	{
		// bắt đầu timer (nếu chưa start)
		const auto& audio = playing_screen->audio;
		if (!is_started)
		{
			current.object_script = playing_screen->render.mapset->get_render_scripts()->begin();

			audio.mixer->music.play(audio.beatmap_music->get(playing_screen->mapset->general.audio_file));
			is_started = true;
			resume();
			return;
		}

		const auto current_time = system.timer.get_time();
		if (current_time > audio.mixer->music.get_position()) 
			Structures::Audio::Bus<Types::Audio::Music>::seek(current_time);

		update_input(current_time, events);
		auto click_left = system.key_stroke.get_recently_pressed_left();
		auto click_right = system.key_stroke.get_recently_pressed_right();

		update_timing(current_time);
		update_object(current_time, click_left, click_right); //TODO: Xử lý fail ở đây
		update_render(current_time);
		update_event_and_action(current_time);
	}
	PlayingScreen::Logic::Logic(
		PlayingScreen* playing_screen,
		const float* mod_multiplier,
		const bool* no_fail)
		: playing_screen(playing_screen), current(*playing_screen->mapset), system(*playing_screen->mapset, no_fail, mod_multiplier)
	{
	}
	PlayingScreen::Logic::Current::Current(const Game::Beatmap::Mapset& mapset)
		: beatmap_sample(&mapset.general.sample_set)
	{
	}
	PlayingScreen::Logic::System::System(
		const Game::Beatmap::Mapset& mapset,
		const bool* no_fail, const float* mod_multiplier)
		: score(mapset, mod_multiplier), health(mapset, timer.is_paused(), no_fail)
	{
	}

	//! Render
	PlayingScreen::Render::Render(
		PlayingScreen* playing_screen,
		const Memory& memory, const bool load_storyboard)
		: playing_screen(playing_screen)
	{
		// Setup
		this->mapset = std::make_shared<Mapset::Render::Mapset>(memory, *playing_screen->mapset);
		cursor = std::make_shared<Cursor::Render::Cursor>(memory);
		health = std::make_shared<Health::Render::Health>(memory, playing_screen->logic.system.health.get_health());
		score = std::make_shared<Score::Render::Score>(memory, playing_screen->logic.system.score.get_score(),
			playing_screen->logic.system.score.accuracy.get_accuracy(),
			playing_screen->logic.system.score.combo.get_current_combo());
		if (load_storyboard)
		{
			Work::Render::Layers::storyboard->resize(playing_screen->mapset->general.widescreen_storyboard);
			this->storyboard = std::make_unique<Game::Beatmap::Event::EventObjects>(
				playing_screen->mapset->events, Work::Render::renderer,
				playing_screen->mapset->path.parent_path(), &playing_screen->logic.system.timer,
				playing_screen->action_buffer, &playing_screen->event_buffer);
			this->storyboard->submit_to_buffer(
				Work::Render::Layers::normal.get(), &Work::Render::Layers::storyboard->background,
				&Work::Render::Layers::storyboard->fail, &Work::Render::Layers::storyboard->pass,
				&Work::Render::Layers::storyboard->foreground);
		}

		// Submit lên on_before_render buffer của layers
		Work::Render::Layers::playground->render_buffer.add(this->mapset);
		Work::Render::Layers::static_hud->render_buffer.add(this->cursor);
		Work::Render::Layers::hud->render_buffer.add(this->health);
		Work::Render::Layers::hud->render_buffer.add(this->score);
	}

	//! Audio
	void PlayingScreen::Audio::check_and_play_sound(
		const int64_t& current_time,
		const Game::Beatmap::HitObjects::Floor& floor,
		const Types::Game::Gameplay::NoteScore& score,
		const Game::Beatmap::Hitsound::TimingSample& timing_sample, const Game::Beatmap::Hitsound::HitSampleType& beatmap_sample) const
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
			auto hit_sound_event = std::make_shared<Events::Event::Playing::Hitsound>(floor, *playing_screen->mapset, *beatmap_effect);
			playing_screen->event_buffer.add(current_time, std::move(hit_sound_event));
		}

		if (floor.second_hit_sound.has_value() && floor.second_hit_sample.has_value())
		{
			const auto second_sound_files =
				Game::Beatmap::Hitsound::get_hit_sound_filename(floor.second_hit_sound.value(), floor.second_hit_sample.value(), timing_sample, beatmap_sample, *beatmap_effect);
			for (const auto& sound_name : second_sound_files)
			{
				auto sound = beatmap_effect->find(sound_name, true); // beatmap
				if (!sound.is_valid()) sound = skin_effect->find(sound_name); // skin
				mixer->effect.play(sound, Game::Beatmap::Hitsound::get_hit_sample_volume(floor.second_hit_sample.value(), timing_sample));
				auto hit_sound_event = std::make_shared<Events::Event::Playing::Hitsound>(floor, *playing_screen->mapset, *beatmap_effect);
				playing_screen->event_buffer.add(current_time, std::move(hit_sound_event));
			}
		}
	}
	PlayingScreen::Audio::Audio(
		PlayingScreen* playing_screen,
		const std::filesystem::path& mapset_root,
		Structures::Audio::Mixer* mixer,
		Structures::Audio::MusicMemory* beatmap_music,
		Structures::Audio::EffectMemory* beatmap_effect,
		Structures::Audio::EffectMemory* skin_effect)
		: playing_screen(playing_screen), mixer(mixer), beatmap_music(beatmap_music), beatmap_effect(beatmap_effect), skin_effect(skin_effect)
	{
		const auto beatmap_audio_file = mapset_root / playing_screen->mapset->general.audio_file;

		beatmap_music->load(beatmap_audio_file, beatmap_audio_file.filename().string());
		beatmap_effect->load(mapset_root, mapset_root);
	}

	//! Screen
	PlayingScreen::PlayingScreen(
		const std::filesystem::path& mapset_path,
		const float& mod_multiplier,
		const bool load_storyboard,
		const bool no_fail, const bool auto_play)
		: mapset(std::make_unique<const Game::Beatmap::Mapset>(mapset_path, load_storyboard)),
		mod_multiplier(mod_multiplier), no_fail(no_fail), auto_play(auto_play),
		logic(this, &this->mod_multiplier, &this->no_fail),
		render(this, *Work::Render::Textures::skin, load_storyboard),
		audio(this, mapset_path.parent_path(),
			Work::Audio::mixer.get(),
			&Work::Audio::Memory::Beatmap::music, 
			&Work::Audio::Memory::Beatmap::effect,
			&Work::Audio::Memory::Skin::effect)
	{
	}
}