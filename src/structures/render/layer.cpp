﻿#include "structures/render/layer.h" // Header
#include <ranges>
#include "config.h"

//! Layers
using namespace Structures::Render::Layers;
// ::Layer::LayerCamera
uint8_t Layer::LayerCamera::LayerCamera::get_alpha() const { return alpha; }
void Layer::LayerCamera::LayerCamera::set_alpha(const uint8_t& value) { alpha = value; }
void Layer::LayerCamera::LayerCamera::move_x(const float& dx) { render_pos.x += dx; }
void Layer::LayerCamera::LayerCamera::move_y(const float& dy) { render_pos.y += dy; }
SDL_FPoint Layer::LayerCamera::LayerCamera::get_camera_pos() const { return render_pos; }
SDL_FPoint Layer::LayerCamera::LayerCamera::get_camera_size(const bool after_scale) const
{
	return after_scale
		? SDL_FPoint{ GameConfig::Video::Camera::DEFAULT_SIZE_WIDTH * scale.x, GameConfig::Video::Camera::DEFAULT_SIZE_HEIGHT * scale.y }
	: SDL_FPoint{ GameConfig::Video::Camera::DEFAULT_SIZE_WIDTH, GameConfig::Video::Camera::DEFAULT_SIZE_HEIGHT };
}
void Layer::LayerCamera::LayerCamera::move_into_camera(RenderObjects::RenderObject& object) const
{
	const auto render_pos_sdl = origin_pos.convert_pos_to_origin(render_pos, { 0, 0 });
	object.config.render_pos.x -= render_pos_sdl.x;
	object.config.render_pos.y -= render_pos_sdl.y;
}
void Layer::LayerCamera::LayerCamera::move_out_camera(RenderObjects::RenderObject& object) const
{
	const auto render_pos_sdl = origin_pos.convert_pos_to_origin(render_pos, { 0, 0 });
	object.config.render_pos.x += render_pos_sdl.x;
	object.config.render_pos.y += render_pos_sdl.y;
}

// ::Layer
void Layer::render()
{
	if (!memory || !visible || render_buffer.empty()) return;
	const auto size = static_cast<int32_t>(render_buffer.size());
	auto begin = render_buffer.begin();
	auto end = render_buffer.end();
	if (render_range.has_value())
	{
		if (render_range.value().first > render_range.value().second)
			std::swap(render_range.value().first, render_range.value().second);
		if (0 <= render_range.value().first && render_range.value().first < size)
			begin = std::next(render_buffer.begin(), render_range.value().first);
		if (0 <= render_range.value().second && render_range.value().second < size)
			end = std::next(render_buffer.begin(), render_range.value().second + 1);
	}
	for (auto objects = begin; objects != end; ++objects)
	{
		for (auto& object : *objects)
		{
			// i hate this job fk
			camera.move_into_camera(object);
			object.render();
			camera.move_out_camera(object);
		}
	}
}
Layer::Layer(const TextureMemory* memory) : memory(memory) {}
void Layer::reset(const bool to_initial_state)
{
	render_buffer.clear();
	render_range.reset();
	if (to_initial_state)
	{
		camera = LayerCamera();
	}
}

// ::PlaygroundLayer
void PlaygoundLayer::run_beatmap(
	const GameObjects::HitObjects::HitObjects& hit_objects,
	const GameObjects::Metadata::CalculatedDifficulty& difficulty,
	const GameObjects::Timing::TimingPoints& timing_points)
{
	reset();

	auto current_uninherited = timing_points.begin();
	auto current_inherited = timing_points.begin();
	RenderObjects::Playground::RenderHitObject previous;
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
				hit_object, *memory, difficulty, current_inherited->second.get_velocity(), &previous);
			render_buffer.push_back(previous);
			break;
		case Template::Game::HitObject::HitObjectType::SLIDER:
			previous = RenderObjects::Playground::RenderSlider(
				hit_object, *memory, difficulty, current_uninherited->second.beat_length,
				current_inherited->second.get_velocity(), &previous);
			render_buffer.push_back(previous);
			break;
		}
	}
}
PlaygoundLayer::PlaygoundLayer(const TextureMemory* target_memory): Layer(target_memory) {}
PlaygoundLayer::PlaygoundLayer(
	const TextureMemory* target_memory,
	const GameObjects::HitObjects::HitObjects& hit_objects, 
	const GameObjects::Metadata::CalculatedDifficulty& difficulty, 
	const GameObjects::Timing::TimingPoints& timing_points) : Layer(target_memory)
{
	run_beatmap(hit_objects, difficulty, timing_points);
}