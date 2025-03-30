#pragma once
#include <SDL3/SDL_render.h>
#include "rule/config.h"
#include "texture.h"
#include "object.h"
#include "utilities.h"

namespace Layers
{
	/**
	 * Trong SDL3, thật buồn là ta ko có một cấu trúc nào để biểu diễn một Layer :(((\n
	 * Vì vậy, ý tưởng của mình là tạo một cấu trúc quản lý các render_buffer giống như Layer,
	 * và render trực tiếp lên màn hình.
	 * Cấu trúc này sẽ có config chung để có thể đồng loạt thay đổi các render_buffer. \n
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
		using RenderRange = std::pair<RenderBuffer::iterator, RenderBuffer::iterator>;

		SDL_Renderer* renderer;
		RenderBuffer render_buffer;
		std::optional<RenderRange> render_range;
		RenderConfig render_setting;
		TextureMemory memory;
		bool visible = true;
		struct Camera
		{
			SDL_FPoint centre_pos = {
				Utilities::Math::centre(0, ImmutableConfig::Video::LOGICAL_WIDTH),
				Utilities::Math::centre(0, ImmutableConfig::Video::LOGICAL_HEIGHT)
			};
			float zoom = 1.0f;

			void move_into_camera(Texture::TextureConfig& object) const;
			void move_out_camera(Texture::TextureConfig& object) const;
		} camera;

		virtual ~Layer() = default;
		explicit Layer(SDL_Renderer* renderer);
		virtual void render();
		void clear(bool to_initial_state = false);
	};
}