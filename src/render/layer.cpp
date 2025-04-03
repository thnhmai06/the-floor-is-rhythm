#include "render/layer.h" // Header
#include <ranges>
#include "config.h"

//! LayerCamera
uint8_t LayerCamera::get_alpha() const { return alpha; }
void LayerCamera::set_alpha(const uint8_t& value) { alpha = value; }
float LayerCamera::get_zoom() const { return scale.x; }
void LayerCamera::set_zoom(const float& value) { scale = { .x= value, .y= value }; }
void LayerCamera::move_x(const float& dx) { render_pos.x += dx; }
void LayerCamera::move_y(const float& dy) { render_pos.y += dy; }
SDL_FPoint LayerCamera::get_camera_pos() const { return render_pos; }
SDL_FPoint LayerCamera::get_camera_size(const bool after_scale) const
{
	return after_scale
	? SDL_FPoint{ GameConfig::Video::DEFAULT_CAMERA_SIZE_WIDTH * scale.x, GameConfig::Video::DEFAULT_CAMERA_SIZE_HEIGHT * scale.y }
	: SDL_FPoint{ GameConfig::Video::DEFAULT_CAMERA_SIZE_WIDTH, GameConfig::Video::DEFAULT_CAMERA_SIZE_HEIGHT };
}
void LayerCamera::move_into_camera(RenderObjects::RenderObject& object) const
{
	object.config.scale.x *= scale.x;
	object.config.scale.y *= scale.y;
	const auto camera_pos_sdl = origin_pos.convert_pos_from_origin(get_camera_pos());
	object.config.render_pos.x -= camera_pos_sdl.x;
	object.config.render_pos.y -= camera_pos_sdl.y;
}
void LayerCamera::move_out_camera(RenderObjects::RenderObject& object) const
{
	object.config.scale.x /= scale.x;
	object.config.scale.y /= scale.y;
	const auto camera_pos_sdl = origin_pos.convert_pos_from_origin(get_camera_pos());
	object.config.render_pos.x += camera_pos_sdl.x;
	object.config.render_pos.y += camera_pos_sdl.y;
}

//! Layer
void Layers::Layer::render()
{
	if (!visible) return;
	const auto begin = render_range ? render_range->first : render_buffer.begin();
	const auto end = render_range ? render_range->second : render_buffer.end();
	for (auto objects = begin; objects != end; ++objects)
	{
		for (auto& object: *objects)
		{
			// i hate this job fk
			camera.move_into_camera(object);
			object.render();
			camera.move_out_camera(object);
		}
	}
}
Layers::Layer::Layer(SDL_Renderer* renderer) : renderer(renderer), memory(renderer) {}
void Layers::Layer::clear(const bool to_initial_state)
{
	render_buffer.clear();
	render_range.reset();
	if (to_initial_state)
	{
		camera = LayerCamera();
		memory.free_all();
	}
}

//! PlaygroundLayer
void Layers::PlaygoundLayer::run_beatmap(
	const GameObjects::HitObjects::HitObjects& hit_objects,
	const GameObjects::Metadata::CalculatedDifficulty& difficulty,
	const GameObjects::Timing::TimingPoints& timing_points)
{
	clear();

	auto current_uninherited = timing_points.begin();
	auto current_inherited = timing_points.begin();
	RenderObjects::Playground::RenderHitobject previous;
	for (const auto& hit_object : hit_objects | std::views::values)
	{
		auto next_timing_point = std::next(current_uninherited, 1);
		while (next_timing_point != timing_points.end() &&
			hit_object.get_time() >= next_timing_point->first)
		{
			if (next_timing_point->second.beat_length < 0) // inherited
				current_inherited = next_timing_point;
			else // uninherited
				current_uninherited = next_timing_point;

			++next_timing_point;
		}

		switch (hit_object.get_type())
		{
		case Template::Game::HitObject::HitObjectType::FLOOR:
			previous = RenderObjects::Playground::RenderFloor(
				hit_object, memory, difficulty, current_inherited->second.get_velocity(), &previous);
			render_buffer.push_back(previous);
			break;
		case Template::Game::HitObject::HitObjectType::SLIDER:
			previous = RenderObjects::Playground::RenderSlider(
				hit_object, memory, difficulty, current_uninherited->second.beat_length,
				current_inherited->second.get_velocity(), &previous);
			render_buffer.push_back(previous);
			break;
		}
	}
}

Layers::PlaygoundLayer::PlaygoundLayer(SDL_Renderer* renderer): Layer(renderer) {}
Layers::PlaygoundLayer::PlaygoundLayer(
	SDL_Renderer* renderer, 
	const GameObjects::HitObjects::HitObjects& hit_objects, 
	const GameObjects::Metadata::CalculatedDifficulty& difficulty, 
	const GameObjects::Timing::TimingPoints& timing_points) : Layer(renderer)
{
	run_beatmap(hit_objects, difficulty, timing_points);
}

//void Layers::Layer::to_absolute_object(Texture::TextureConfig& object) const
//{
//	// dstrect
//	if (render_setting.dst_rect && object.origin_dst)
//	{
//		// Position
//		object.origin_dst->x += render_setting.dst_rect->x;
//		object.origin_dst->y += render_setting.dst_rect->y;
//
//		// Scaling
//		object.origin_dst->w *= render_setting.dst_rect->w / GameConfig::Video::LOGICAL_WIDTH;
//		object.origin_dst->h *= render_setting.dst_rect->h / GameConfig::Video::LOGICAL_HEIGHT;
//	}
//	// alpha
//	object.alpha = (object.alpha * render_setting.alpha) / 255;
//}
//void Layers::Layer::to_relative_object(Texture::TextureConfig& object) const
//{
//	// dstrect
//	if (render_setting.dst_rect && object.origin_dst)
//	{
//		// Position
//		object.origin_dst->x -= render_setting.dst_rect->x;
//		object.origin_dst->y -= render_setting.dst_rect->y;
//		// Scaling
//		object.origin_dst->w *= static_cast<float>(GameConfig::Video::LOGICAL_WIDTH) / render_setting.dst_rect->w;
//		object.origin_dst->h *= static_cast<float>(GameConfig::Video::LOGICAL_HEIGHT) / render_setting.dst_rect->h;
//	}
//	// alpha
//	object.alpha = (object.alpha * 255) / render_setting.alpha;
//}