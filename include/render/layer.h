//! Chưa test
#pragma once
#include <string>
#include <SDL3/SDL_render.h>
#include "texture.h"

/**
 * Trong SDL3, thật buồn là ta ko có một cấu trúc nào để biểu diễn một Layer :(((\n
 * Vì vậy, ý tưởng của mình là tạo một cấu trúc quản lý các objects giống như Layer,
 * và render trực tiếp lên màn hình.
 * Cấu trúc này sẽ có Config chung để có thể đồng loạt thay đổi các objects. \n
 *
 * Tại sao không dùng SDL_Texture* đại diện cho Layer? \n
 * Vì nó sẽ render texture ra Layer đó, rồi mới render Layer ra
 * màn hình chính. Như vậy giống như việc ta render 2 lần vậy.
 * (Vấn đề về Hiệu suất và Overhead)
 */
struct Layer
{
private:
	void to_absolute_object(TextureRenderConfig& object) const;
	void to_relative_object(TextureRenderConfig& object) const;

public:
	SDL_Renderer* renderer;
	struct Config
	{
		SDL_FRect* dst_rect = nullptr;
		uint8_t alpha = 255;
	} config;
	TextureMemory memory;
	std::list<std::pair<std::string, TextureRenderConfig>> objects;

	void render();
	explicit Layer(SDL_Renderer* renderer);
	void free(bool to_initial_state = true);
};