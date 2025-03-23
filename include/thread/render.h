#pragma once
#include "render/layer.h"
#include "render/texture.h"
#include "utilities.h"

namespace RenderThread
{
	constexpr char NAME[] = "RenderThread";
	int32_t worker(void* window);
	SDL_Thread* start_thread(void* window);
}

namespace LayerSet
{
	namespace Background
	{
		inline std::unique_ptr<TextureMemory> memory;
		inline std::unique_ptr<Layer> layer;

		static void init(SDL_Renderer* renderer)
		{
			Utilities::Render::create_memory(memory, renderer);
			Utilities::Render::create_layer(layer, renderer, memory.get());
		}
		static void free()
		{
			layer->free(false);
			memory->clear();
		}
	}
	namespace Playground
	{
		inline std::unique_ptr<TextureMemory> memory;
		inline std::unique_ptr<Layer> layer;

		static void init(SDL_Renderer* renderer)
		{
			Utilities::Render::create_memory(memory, renderer);
			Utilities::Render::create_layer(layer, renderer, memory.get());
		}
		static void free()
		{
			layer->free(false);
			memory->clear();
		}
	}
	namespace HUD
	{
		inline std::unique_ptr<TextureMemory> memory;
		inline std::unique_ptr<Layer> layer;

		static void init(SDL_Renderer* renderer)
		{
			Utilities::Render::create_memory(memory, renderer);
			Utilities::Render::create_layer(layer, renderer, memory.get());
		}
		static void free()
		{
			layer->free(false);
			memory->clear();
		}
	}

	static void init(SDL_Renderer* renderer)
	{
		Background::init(renderer);
		Playground::init(renderer);
		HUD::init(renderer);
	}
	static void render()
	{
		static_cast<void>(Background::layer->re_draw());
		static_cast<void>(Playground::layer->re_draw());
		static_cast<void>(HUD::layer->re_draw());
	}
	static void free()
	{
		Background::free();
		Playground::free();
		HUD::free();
	}
}