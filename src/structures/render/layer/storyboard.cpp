#include "structures/render/layer/storyboard.h" // Header
#include "config.h"

namespace Structures::Render::Layer
{
	using namespace ::Config::Game::Render;
	using namespace Utilities::Math::FPoint;

	const bool& StoryboardLayer::get_widescreen_mode() const { return widescreen; }
	void StoryboardLayer::resize(const bool widescreen, const bool force)
	{
		if (!force && widescreen == this->widescreen) return;
		this->widescreen = widescreen;
		SDL_Point window_size;
		Object::Object::Config::OriginPoint grid_origin;
		SDL_FPoint grid_size;
		SDL_GetCurrentRenderOutputSize(memory.renderer, &window_size.x, &window_size.y);
		if (!widescreen)
		{
			grid_origin = { 0, 0 };
			grid_size = STORYBOARD_GRID_SIZE;
		}
		else
		{
			grid_origin = (STORYBOARD_GRID_WIDESCREEN_SIZE - STORYBOARD_GRID_SIZE) / 2;
			grid_size = STORYBOARD_GRID_WIDESCREEN_SIZE;
		}
		const auto [scale_x, scale_y] = to_float_point(window_size) / grid_size;
		const auto final_scale = std::min(scale_x, scale_y);
		const auto texture_size = to_integer_point(grid_size * final_scale);
		grid_origin *= final_scale;

		background.camera.get_camera_origin() = fail.camera.get_camera_origin() =
			pass.camera.get_camera_origin() = foreground.camera.get_camera_origin() = grid_origin;
		background.grid_size = fail.grid_size = pass.grid_size = foreground.grid_size = grid_size;
		memory.create_new(name_background, texture_size);
		memory.create_new(name_fail, texture_size);
		memory.create_new(name_pass, texture_size);
		memory.create_new(name_foreground, texture_size);
	}
	void StoryboardLayer::render()
	{
		//! Thứ tự: background -> fail -> pass -> foreground
		if (!visible) return;

		//! Render Texture
		SDL_Texture* const current_target = SDL_GetRenderTarget(memory.renderer);
		background.render_no_change_back_target(true);
		fail.render_no_change_back_target(true);
		pass.render_no_change_back_target(true);
		foreground.render_no_change_back_target(true);
		SDL_SetRenderTarget(memory.renderer, current_target);

		//! Render chính
		// Letter Box, căn giữa
		const auto texture = background.target_texture.get_size();
		SDL_Point window_int; SDL_GetCurrentRenderOutputSize(memory.renderer, &window_int.x, &window_int.y);
		const SDL_FPoint window = to_float_point(window_int);

		const auto [scale_x, scale_y] = window / texture;
		const auto final_scale = std::min(scale_x, scale_y);
		const SDL_FPoint dst_size = texture * final_scale;
		const SDL_FPoint dst_pos = -Utilities::Math::centre(dst_size, window);
		const SDL_FRect dst_rect = Utilities::Render::merge_pos_size(dst_pos, dst_size);
		background.target_texture.render(nullptr, &dst_rect);
		fail.target_texture.render(nullptr, &dst_rect);
		pass.target_texture.render(nullptr, &dst_rect);
		foreground.target_texture.render(nullptr, &dst_rect);
	}
	void StoryboardLayer::clear()
	{
		background.clear();
		fail.clear();
		pass.clear();
		foreground.clear();
	}
	StoryboardLayer::StoryboardLayer(SDL_Renderer* renderer)
		: memory(renderer),
		//! Layers
		background(memory.create_new(name_background, { 1, 1 }), { 0, 0 }),
		fail(memory.create_new(name_fail, {1, 1}), { 0, 0 }),
		pass(memory.create_new(name_pass, {1, 1}), { 0, 0 }),
		foreground(memory.create_new(name_foreground, {1, 1}), { 0, 0 })
	{
		resize(widescreen, true);
	}
}