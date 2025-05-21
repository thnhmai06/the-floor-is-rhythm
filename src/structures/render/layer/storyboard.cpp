#include "structures/render/layer/storyboard.h" // Header
#include "config.h"

namespace Structures::Render::Layer
{
	using namespace ::Config::Game::Render;
	using namespace Utilities::Math::FPoint;
	using Engine::Render::Object::Object;

	const bool& Storyboard::get_widescreen_mode() const { return widescreen; }
	void Storyboard::resize(const bool widescreen, const bool force)
	{
		if (!force && widescreen == this->widescreen) return;
		this->widescreen = widescreen;
		SDL_Point window_size;
		Engine::Render::Config::OriginPoint grid_origin;
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

		layer.config->render_pos = grid_origin;
		layer.grid_size = grid_size;
		memory.create_new(name_texture, texture_size);
	}
	void Storyboard::render()
	{
		//! CHÚ Ý: KHÔNG TÁCH TEXTURE LAYER RA LÀM 4 LAYER KHÁC NHAU - NHƯ VẬY SẼ
		//! LÀM HỎNG ADD BLEND MODE!
		//! Thứ tự: background -> fail -> pass -> foreground

		//! Render về mặt Texture
		layer.target_texture.clear();
		layer.render();

		//! Render chính
		const auto texture = layer.target_texture.get_size();
		SDL_Point window_int; SDL_GetCurrentRenderOutputSize(memory.renderer, &window_int.x, &window_int.y);
		const SDL_FPoint window = to_float_point(window_int);
		// Letter Box, căn giữa
		const auto [scale_x, scale_y] = window / texture;
		const auto final_scale = std::min(scale_x, scale_y);
		const SDL_FPoint dst_size = texture * final_scale;
		const SDL_FPoint dst_pos = -Utilities::Math::centre(dst_size, window);
		const SDL_FRect dst_rect = Utilities::Render::merge_pos_size(dst_pos, dst_size);
		layer.target_texture.render(nullptr, &dst_rect);
	}
	void Storyboard::clear()
	{
		layer.clear();
	}
	Storyboard::Storyboard(SDL_Renderer* renderer)
		: memory(renderer),
		//! Layers
		layer(memory.create_new(name_texture, { 1, 1 }), { 0, 0 })
	{
		resize(widescreen, true);
	}
}