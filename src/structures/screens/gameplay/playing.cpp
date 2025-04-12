#include "structures/screens/gameplay/playing.h" // Header
#include "structures/render/layers/objects/gameplay/beatmap.h"
#include "structures/render/layers/objects/gameplay/cursor.h"
#include "work/render/layers.h"
#include "work/render/textures.h"
#include "utilities.h"

//! PLayingScreen
namespace Structures::Screens::Gameplay
{
	// ::Logic::Score::count
	float PlayingScreen::Logic::Score::Count::get_accuracy() const
	{
		if (count_300 + count_100 + count_50 + count_slider_tick + count_miss == 0) return 100.00f; // tránh chia 0

		// chia 10 để giảm số cần nhớ
		constexpr uint8_t SCORE_PERFECT = 30; // 300
		constexpr uint8_t SCORE_GREAT = 10; // 100
		constexpr uint8_t SCORE_BAD = 5; // 50
		constexpr uint8_t SCORE_SLIDER_TICK = 1; // 10

		const unsigned long total = count_300 * SCORE_PERFECT + count_100 * SCORE_GREAT + count_50 * SCORE_BAD + count_slider_tick * SCORE_SLIDER_TICK;
		const unsigned long total_if_perfect = (count_300 + count_100 + count_50 + count_miss) * SCORE_PERFECT + count_slider_tick * SCORE_SLIDER_TICK;
		return static_cast<float>(total) / static_cast<float>(total_if_perfect);
	}
	unsigned long PlayingScreen::Logic::Score::Count::get_elapsed_objects_num() const
	{
		return count_300 + count_100 + count_50 + count_miss;
	}
	// ::Logic::Score
	unsigned long PlayingScreen::Logic::Score::update_score()
	{
		// Score = ((700000 * max_combo / total_combo) + (300000 * accuracy ^ 10 * elapsed_objects / total_objects)) * mod_multiplier

		constexpr float BASE_COMBO = 700000;
		constexpr float BASE_ACCURACY = 300000;

		const float accuracy = count.get_accuracy();
		const float elapsed_objects = static_cast<float>(count.get_elapsed_objects_num());
		const unsigned long combo_score = static_cast<unsigned long>
			(BASE_COMBO * static_cast<float>(max_combo) / static_cast<float>(total_combo));
		const unsigned long accuracy_score = static_cast<unsigned long>
			(BASE_ACCURACY * std::powf(accuracy, 10) * elapsed_objects / static_cast<float>(total_objects_num));
		const unsigned long score_no_multiplier = static_cast<unsigned long>
			(static_cast<float>(combo_score + accuracy_score) * mod_multiplier);
		return this->score = static_cast<unsigned long>
			(static_cast<float>(score_no_multiplier) * mod_multiplier);
	}
	PlayingScreen::Logic::Score::Score(const Game::Beatmap::Beatmap& beatmap, const float& mod_multiplier)
		: total_objects_num(beatmap.total_objects_num), total_combo(beatmap.total_combo), mod_multiplier(mod_multiplier)
	{
	}
	// ::Logic
	PlayingScreen::Logic::Logic(const Game::Beatmap::Beatmap* beatmap, const int64_t& start_time, const float& mod_multiplier)
		: beatmap(beatmap), timer(start_time), score(*beatmap, mod_multiplier) {
	}

	// ::Render
	PlayingScreen::Render::Components::Components(
		Storage* storage,
		Layer* playground_layer, Layer* cursor_layer) :
		map_set(storage, &playground_layer->render_buffer),
		cursor(storage, &cursor_layer->render_buffer) {
	}
	PlayingScreen::Render::Render(
		const Types::Game::Direction::Direction* current_direction,
		Layer& playground_layer,
		Layer& cursor_layer,
		const TextureMemory& skin,
		const Game::Beatmap::Beatmap& beatmap) : components(&storage, &playground_layer, &cursor_layer)
	{
		//! Beatmap
		components.map_set.storage_item = storage.insert(
			Utilities::Code::get_last_element_iterator(storage),
			std::make_unique<Structures::Render::Objects::Gameplay::Beatmap::Collection>(skin, beatmap.hit_objects, beatmap.calculated_difficulty, beatmap.timing_points)
		);
		components.map_set.render_item = playground_layer.render_buffer.add_collection(components.map_set.storage_item->get()); // Thêm vào render_buffer ở đây nè :D
		//TODO: set render range
		components.map_set.storage_item->get()->render_range = { {0, 10} }; // for testing

		//! Cursor
		components.cursor.storage_item = storage.insert(
			Utilities::Code::get_last_element_iterator(storage),
			std::make_unique<Structures::Render::Objects::Gameplay::Cursor::Collection>(skin, current_direction)
		);
		components.cursor.render_item = cursor_layer.render_buffer.add_collection(components.cursor.storage_item->get()); // Thêm vào render_buffer ở đây nè :D
	}
	// total
	PlayingScreen::PlayingScreen(const char* beatmap_path) :
		beatmap(std::make_unique<const Game::Beatmap::Beatmap>(beatmap_path)),
		logic(beatmap.get()),
		render(&logic.key_stoke.direction.current_direction,
			*Work::Render::Layers::playground, *Work::Render::Layers::cursor,
			*Work::Render::Textures::skin, *this->beatmap)
	{
		//TODO: Tìm StartTime đẻ nhét vào logic.timer
	}
}