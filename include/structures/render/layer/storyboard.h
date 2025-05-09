#pragma once
#include "structures/render/layer.h"

namespace Structures::Render::Layer
{
	// Đây là Layer đặc biệt, khác hoàn toàn so với Layer thường.
	// Để đơn giản và gọn nhẹ, ta sẽ không thêm mấy cái thành phần
	// on_before_render của engine mà sẽ sử dụng trực tiếp SDL Render System.
	//
	// Gồm 4 texture layer con: background, fail, pass, foreground
	// https://osu.ppy.sh/wiki/en/Storyboard/Scripting/General_Rules#:~:text=55%E2%80%93440-,Layers,-All%20storyboard%20sprites
	struct StoryboardLayer
	{
	protected:
		static constexpr char name_background[] = "background";
		static constexpr char name_fail[] = "fail";
		static constexpr char name_pass[] = "pass";
		static constexpr char name_foreground[] = "foreground";

		Memory memory;
		bool widescreen = false;

	public:
		TextureLayer background, fail, pass, foreground;

		const bool& get_widescreen_mode() const;
		//! Hạn chế dùng hàm này vì hàm này phải tạo lại texture (đã kiểm tra trùng lặp)
		void resize(bool widescreen, bool force = false);
		void render();
		void clear() const;
		void reset();

		explicit StoryboardLayer(SDL_Renderer* renderer);
	};
}