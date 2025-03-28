#pragma once
#include <SDL3/SDL_render.h>
#include "utilities.h"
#include "rule/config.h"
#include "texture.h"
#include "playground/r_object.h"

namespace Layers
{
	/**
	 * Trong SDL3, thật buồn là ta ko có một cấu trúc nào để biểu diễn một Layer :(((\n
	 * Vì vậy, ý tưởng của mình là tạo một cấu trúc quản lý các render_buffer giống như Layer,
	 * và render trực tiếp lên màn hình.
	 * Cấu trúc này sẽ có LayerConfig chung để có thể đồng loạt thay đổi các render_buffer. \n
	 *
	 * Tại sao không dùng SDL_Texture* đại diện cho Layer? \n
	 * Vì nó sẽ render texture ra Layer đó, rồi mới render Layer ra
	 * màn hình chính. Như vậy giống như việc ta render 2 lần vậy.
	 * (Vấn đề về Hiệu suất và Overhead)
	 */
	struct Layer
	{
	private:
		void to_absolute_object(Texture::TextureConfig& object) const;
		void to_relative_object(Texture::TextureConfig& object) const;

	public:
		using RenderBuffer = std::list<RenderObjects::RenderObject>;

		SDL_Renderer* renderer;
		bool visible = true;
		struct LayerConfig
		{
			SDL_FRect* dst_rect = nullptr;
			uint8_t alpha = 255;
		} config;
		struct Camera
		{
			SDL_FPoint centre_pos = {
				Utilities::Math::centre(0, ImmutableConfig::Video::LOGICAL_WIDTH),
				Utilities::Math::centre(0, ImmutableConfig::Video::LOGICAL_HEIGHT)
			};
			float zoom = 1;

			void move_into_camera(Texture::TextureConfig& object) const;
			void move_out_camera(Texture::TextureConfig& object) const;
		} camera;
		Texture::TextureMemory memory;
		RenderBuffer render_buffer;
		std::optional<std::pair<RenderBuffer::iterator, RenderBuffer::iterator>> render_range;

		virtual ~Layer() = default;
		explicit Layer(SDL_Renderer* renderer);
		virtual void render();
		void clear(bool to_initial_state = false);
	};
}