#include "structures/render/screens/gameplay/playing.h" // Header
#include "structures/render/layers/objects/gameplay/mapset.h"
#include "structures/render/layers/objects/gameplay/cursor.h"

using namespace Structures::Render::Screen::Gameplay;

// ::PlayingScreen
// :: Components
PlayingScreen::Components::Components(
	RenderObjects::RenderObjectStorage* storage,
	Layers::Layer* playground_layer, Layers::Layer* cursor_layer) :
	map_set(storage, &playground_layer->render_buffer),
	cursor(storage, &cursor_layer->render_buffer) {
}
// ::
PlayingScreen::PlayingScreen(
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
		std::make_unique<CursorCollection>(skin, &current_direction)
	);
	components.cursor.render_item = cursor_layer.render_buffer.add_collection(components.cursor.storage_item->get());
}
