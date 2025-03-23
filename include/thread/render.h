#pragma once
#include "render/layer.h"
#include "utilities.h"

//! TODO: Vấn đề: Thread mới sẽ chỉ chạy trên CPU!!
// Có thể là do Renderer được tạo trên thread mới nên sẽ dùng CPU thay vì GPU chăng?

namespace RenderThread
{
	constexpr char NAME[] = "RenderThread";
	int32_t worker(void* window);
	SDL_Thread* start_thread(void* window);
}

namespace WorkingLayer
{
	inline std::unique_ptr<Layer> background;
	inline std::unique_ptr<Layer> playground;
	inline std::unique_ptr<Layer> hud;

	static void init(SDL_Renderer* renderer)
	{
		if (!background) background = std::make_unique<Layer>(renderer);
		if (!playground) playground = std::make_unique<Layer>(renderer);
		if (!hud) hud = std::make_unique<Layer>(renderer);
	}
	static void render()
	{
		static_cast<void>(background->re_draw());
		static_cast<void>(playground->re_draw());
		static_cast<void>(hud->re_draw());
	}
	static void free()
	{
		if (background) background->free();
		if (playground) playground->free();
		if (hud) hud->free();
	}
}