//! Chưa test

#pragma once
#include <string>
#include <vector>
#include <SDL3/SDL_render.h>
#include "texture.h"
#include "logging.h"
#include "config.h"
#include "exceptions.h"

/**
 * Trong SDL3, thật buồn là ta ko có một cấu trúc nào để biểu diễn một Layer :(((\n
 * Vì vậy, ý tưởng của mình là tạo một layer nhưng bản chất là một texture, sau
 * đó chuyển target render lên layer để render những gì muốn vẽ lên layer đó.\n
 * Sau đó chuyển target về window (ban đầu) để render cái layer đó lên.
 *
 * @note - Mặc dù Layer bản chất là Texture, nhưng texture này KHÔNG NÊN được quản lý bởi TextureMemory.\n
 * Hãy để Layer tự quản lý texture này!
 */
struct Layer
{
private:
	// utilities
	static SDL_Texture* create_layer(SDL_Renderer* renderer, const TextureMemory* memory)
	{
		return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,
			Immutable::Video::LOGICAL_WIDTH, Immutable::Video::LOGICAL_HEIGHT);
	}

public:
	SDL_Texture* layer;
	SDL_Renderer* renderer;
	const TextureMemory* memory;
	std::list<std::pair<std::string, TextureRenderConfig>> objects;

	Layer(SDL_Renderer* renderer, const TextureMemory* memory) : renderer(renderer), memory(memory)
	{
		layer = create_layer(renderer, memory);
	}
	void clear(const bool recreate = true)
	{
		SDL_DestroyTexture(layer);
		objects.clear();

		if (recreate)
			layer = create_layer(renderer, memory);
	}
	void draw() const
	{
		SDL_SetRenderTarget(renderer, layer);
		for (const auto& [name, config] : objects)
		{
			if (!memory->render(name, config))
				LOG_ERROR(SDL_Exceptions::Texture::SDL_RenderTexture_Failed(name));
		}
		SDL_SetRenderTarget(renderer, nullptr);
	}
};
