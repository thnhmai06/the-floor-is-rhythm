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
	SDL_Renderer* renderer;
	SDL_Texture* layer;
	TextureRenderConfig config;
	TextureMemory memory;
	std::list<std::pair<std::string, TextureRenderConfig>> objects;

	void free(bool to_initial_state = true);
	bool re_draw() const;
	explicit Layer(SDL_Renderer* renderer);
};
