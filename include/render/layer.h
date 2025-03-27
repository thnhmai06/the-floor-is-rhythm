﻿#pragma once
#include <SDL3/SDL_render.h>
#include "utilities.h"
#include "config.h"
#include "texture.h"
#include "game/hitobject.h"
#include "playground/r_object.h"

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
	bool visible = true;
	struct Config
	{
		SDL_FRect* dst_rect = nullptr;
		uint8_t alpha = 255;
	} config;
	struct Camera
	{
		SDL_FPoint centre = {
			Utilities::Math::centre(0, Immutable::Video::LOGICAL_WIDTH),
			Utilities::Math::centre(0, Immutable::Video::LOGICAL_HEIGHT)
		};
		float zoom = 1;

		void move_into_camera(TextureRenderConfig& object) const;
		void move_out_camera(TextureRenderConfig& object) const;
	} camera;
	TextureMemory memory;
	std::list<RenderObject::RenderObject> objects;

	virtual ~Layer() = default;
	explicit Layer(SDL_Renderer* renderer);
	virtual void render();
	void clear(bool to_initial_state = false);
};

struct PlaygroundLayer final : Layer
{
	std::vector<RenderObject::RenderObject> objects;
	std::pair<unsigned long long, unsigned long long> render_range = { 0, 0 };
	void render() override;

	PlaygroundLayer(SDL_Renderer* renderer, const HitObject::HitObjects& map) : Layer(renderer) {}
};
