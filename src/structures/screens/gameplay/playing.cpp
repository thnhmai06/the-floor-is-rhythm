#include "structures/screens/gameplay/playing.h" // Header
#include "structures/render/layers/objects/gameplay/mapset.h"
#include "structures/render/layers/objects/gameplay/cursor.h"
#include "work/render/layers.h"
#include "work/render/textures.h"

using namespace Structures::Render::Screen::Gameplay;

//! PLayingScreen
// Game
float PlayingScreen::Game::Count::get_accuracy() const
{
	if (count_300 + count_100 + count_50 + count_miss == 0) return 100.00f; // tránh chia 0

	constexpr uint8_t SCORE_PERFECT = 30; // 300
	constexpr uint8_t SCORE_GREAT = 10; // 100
	constexpr uint8_t SCORE_BAD = 5; // 50

	const unsigned long total = count_300 * SCORE_PERFECT + count_100 * SCORE_GREAT + count_50 * SCORE_BAD; // chia 10 để giảm số cần nhớ
	const unsigned long total_if_perfect = (count_300 + count_100 + count_50 + count_miss) * SCORE_PERFECT;
	return static_cast<float>(total) / static_cast<float>(total_if_perfect);
}

// Render
PlayingScreen::Render::Components::Components(
	RenderObjects::RenderObjectStorage* storage,
	Layers::Layer* playground_layer, Layers::Layer* cursor_layer) :
	map_set(storage, &playground_layer->render_buffer),
	cursor(storage, &cursor_layer->render_buffer) {
}
PlayingScreen::Render::Render
(
	const Game& game,
	Layers::Layer& playground_layer,
	Layers::Layer& cursor_layer,
	const Textures::TextureMemory& skin,
	const BeatmapFile& beatmap) : components(&storage, &playground_layer, &cursor_layer)
{
	//! Mapset
	using RenderObjects::Gameplay::Collection::MapsetCollection;
	components.map_set.storage_item = storage.insert(
		std::prev(storage.end(), 1),
		std::make_unique<MapsetCollection>(skin, beatmap.hit_objects, beatmap.calculated_difficulty, beatmap.timing_points)
	);
	components.map_set.render_item = playground_layer.render_buffer.add_collection(components.map_set.storage_item->get());
	//TODO: set render range

	//! Cursor
	using RenderObjects::Cursor::Collection::CursorCollection;
	components.cursor.storage_item = storage.insert(
		std::prev(storage.end(), 1),
		std::make_unique<CursorCollection>(skin, &game.current_direction)
	);
	components.cursor.render_item = cursor_layer.render_buffer.add_collection(components.cursor.storage_item->get());
}

// Total
PlayingScreen::PlayingScreen(const char* beatmap_path) :
	beatmap(std::make_unique<const BeatmapFile>(beatmap_path)),
	render(game, *Work::Render::Layers::playground,
		*Work::Render::Layers::cursor, *Work::Render::Textures::skin, *this->beatmap)
{
}