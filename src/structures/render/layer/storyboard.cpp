#include "structures/render/layer/storyboard.h" // Header

namespace Structures::Render::Layer
{
	using namespace Config::GameConfig::Render;

	const bool& StoryboardLayer::get_widescreen_mode() const { return widescreen; }
	void StoryboardLayer::resize(const bool widescreen)
	{
		if (widescreen == this->widescreen) return;
		this->widescreen = widescreen;

		Object::Object::Config::OriginPoint grid_origin;
		if (!widescreen)
			grid_origin = { 0, 0 };
		else
			grid_origin = {
			(STORYBOARD_GRID_WIDESCREEN_WIDTH - STORYBOARD_GRID_WIDTH) / 2 ,
			(STORYBOARD_GRID_WIDESCREEN_HEIGHT - STORYBOARD_GRID_HEIGHT) / 2 };
		background.camera.get_camera_origin() = fail.camera.get_camera_origin() =
			pass.camera.get_camera_origin() = foreground.camera.get_camera_origin() = grid_origin;

		if (!widescreen)
		{
			memory.create_new(name_background, { STORYBOARD_GRID_WIDTH, STORYBOARD_GRID_HEIGHT });
			memory.create_new(name_fail, { STORYBOARD_GRID_WIDTH, STORYBOARD_GRID_HEIGHT });
			memory.create_new(name_pass, { STORYBOARD_GRID_WIDTH, STORYBOARD_GRID_HEIGHT });
			memory.create_new(name_foreground, { STORYBOARD_GRID_WIDTH, STORYBOARD_GRID_HEIGHT });
		}
		else
		{
			memory.create_new(name_background, { STORYBOARD_GRID_WIDESCREEN_WIDTH, STORYBOARD_GRID_WIDESCREEN_HEIGHT });
			memory.create_new(name_fail, { STORYBOARD_GRID_WIDESCREEN_WIDTH, STORYBOARD_GRID_WIDESCREEN_HEIGHT });
			memory.create_new(name_pass, { STORYBOARD_GRID_WIDESCREEN_WIDTH, STORYBOARD_GRID_WIDESCREEN_HEIGHT });
			memory.create_new(name_foreground, { STORYBOARD_GRID_WIDESCREEN_WIDTH, STORYBOARD_GRID_WIDESCREEN_HEIGHT });
		}
	}
	void StoryboardLayer::render()
	{
		//! Thứ tự: background -> fail -> pass -> foreground

		//! Render Texture
		SDL_Texture* const current_target = SDL_GetRenderTarget(memory.renderer);
		background.render_no_change_back_target(true);
		fail.render_no_change_back_target(true);
		pass.render_no_change_back_target(true);
		foreground.render_no_change_back_target(true);
		SDL_SetRenderTarget(memory.renderer, current_target);

		//! Render chính
		// Letter Box chính giữa
		const auto tex_w = static_cast<float>((widescreen) ? STORYBOARD_GRID_WIDESCREEN_WIDTH : STORYBOARD_GRID_WIDTH);
		const auto tex_h = static_cast<float>((widescreen) ? STORYBOARD_GRID_WIDESCREEN_HEIGHT : STORYBOARD_GRID_HEIGHT);
		int render_w, render_h; SDL_GetCurrentRenderOutputSize(memory.renderer, &render_w, &render_h);
		const float scale = std::min(static_cast<float>(render_w) / tex_w, static_cast<float>(render_h) / tex_h);
		const float dst_w = std::round(tex_w * scale);
		const float dst_h = std::round(tex_h * scale);
		const float offset_x = -Utilities::Math::centre(static_cast<float>(render_w), dst_w);
		const float offset_y = -Utilities::Math::centre(static_cast<float>(render_h), dst_h);
		const SDL_FRect dst_rect = { offset_x, offset_y, dst_w, dst_h };
		background.target_texture.render(nullptr, &dst_rect);
		fail.target_texture.render(nullptr, &dst_rect);
		pass.target_texture.render(nullptr, &dst_rect);
		foreground.target_texture.render(nullptr, &dst_rect);
	}
	void StoryboardLayer::clear() const
	{
		background.target_texture.clear();
		fail.target_texture.clear();
		pass.target_texture.clear();
		foreground.target_texture.clear();
	}
	void StoryboardLayer::reset()
	{
		background.reset(false);
		fail.reset(false);
		pass.reset(false);
		foreground.reset(false);
	}
	StoryboardLayer::StoryboardLayer(SDL_Renderer* renderer)
		: memory(renderer),
		//! Layers
		background(memory.create_new(name_background, { STORYBOARD_GRID_WIDTH, STORYBOARD_GRID_HEIGHT }), { 0, 0 }),
		fail(memory.create_new(name_fail, { STORYBOARD_GRID_WIDTH, STORYBOARD_GRID_HEIGHT }), { 0, 0 }),
		pass(memory.create_new(name_pass, { STORYBOARD_GRID_WIDTH, STORYBOARD_GRID_HEIGHT }), { 0, 0 }),
		foreground(memory.create_new(name_foreground, { STORYBOARD_GRID_WIDTH, STORYBOARD_GRID_HEIGHT }), { 0, 0 })
	{
	}
}