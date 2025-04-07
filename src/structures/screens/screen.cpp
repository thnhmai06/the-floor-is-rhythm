#include "structures/screens/screen.h" // Header
#include "structures/render/playground/mapset.h"


Structures::Screens::PlayingScreen::PlayingScreen(
	Render::Layers::Layer& playground, 
	const Render::Textures::TextureMemory& beatmap_skin, 
	const BeatmapFile& beatmap): map_set()
{
	// Mapset
	using Render::RenderObjects::Playground::Collection::MapsetCollection;
	map_set.storage_item = storage.insert(
		storage.end(),
		MapsetCollection{beatmap_skin, beatmap.hit_objects, beatmap.calculated_difficulty, beatmap.timing_points}
	);
	map_set.render_item = playground.render_buffer.add_collection(&*map_set.storage_item);
	
}
