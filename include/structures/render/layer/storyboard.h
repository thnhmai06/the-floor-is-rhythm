#pragma once
#include "structures/render/layer.h"

namespace Structures::Render::Layer
{
	// Đây là Layer đặc biệt, khác hoàn toàn so với Layer thường.
	// Để đơn giản và gọn nhẹ, ta sẽ không thêm mấy cái thành phần
	// render của engine mà sẽ sử dụng trực tiếp SDL Render System.
	//
	// Gồm 4 texture layer con: background, fail, pass, foreground (ảo)
	// Gộp chung trong 1 layer texture
	// https://osu.ppy.sh/wiki/en/Storyboard/Scripting/General_Rules#:~:text=55%E2%80%93440-,Layers,-All%20storyboard%20sprites
	struct Storyboard
	{
	protected:
		static constexpr auto name_texture = "storyboard";

		Memory memory;
		bool widescreen = false;

	public:
		TextureLayer layer;

		const bool& get_widescreen_mode() const;
		//! Hạn chế dùng hàm này vì hàm này phải tạo lại texture
		//! gây hao phí hiệu suất (đã kiểm tra trùng lặp)
		void resize(bool widescreen, bool force = false);
		void render();
		void clear();

		explicit Storyboard(SDL_Renderer* renderer);
	};
}