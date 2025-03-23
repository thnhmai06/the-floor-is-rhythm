//! Chưa test

#pragma once
#include <string>
#include <SDL3/SDL_render.h>
#include "texture.h"
#include "logging.h"

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
	static SDL_Texture* create_layer(SDL_Renderer* renderer);

public:
	SDL_Texture* layer;
	SDL_Renderer* renderer;
	TextureRenderConfig config;
	const TextureMemory* memory;
	std::list<std::pair<std::string, TextureRenderConfig>> objects;

	Layer(SDL_Renderer* renderer, const TextureMemory* memory);
	void clear(const bool re_create = true);
	[[nodiscard]] bool re_draw() const;
};
