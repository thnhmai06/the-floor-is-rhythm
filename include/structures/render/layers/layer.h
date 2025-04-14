// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
#pragma once
#include "structures/render/layers/objects/collection.h"
#include "utilities.h"

namespace Structures::Render::Layers
{
	using namespace Structures::Render::Textures;

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
		// buffer
		struct RenderBuffer : std::list<const Objects::Collection*>
		{
		protected:
			using BASE = std::list<Objects::Collection*>;

		public:
			struct Item
			{
				RenderBuffer* render_buffer;
				iterator item;

				void remove();

				explicit Item(RenderBuffer* render_buffer);
				Item(RenderBuffer* render_buffer, iterator item);
			};

			const Layer* parent;

			Item add_collection(const Objects::Collection* collection);
			static void remove_collection(Item& item);

			RenderBuffer(const Layer* layer) : parent(layer) {}
		};

		// attributes
		RenderBuffer render_buffer;
		struct Camera : private Objects::Object::Config
		{
		protected:
			using BASE = Objects::Object::Config;

		public:
			SDL_FPoint get_object_offset() const;

			[[nodiscard]] uint8_t get_alpha() const;
			[[nodiscard]] SDL_FPoint get_camera_pos() const;
			[[nodiscard]] SDL_FPoint get_camera_size(bool after_scale = true) const;
			void set_alpha(const uint8_t& value);
			void move_x(const float& dx);
			void move_y(const float& dy);

			Camera(const OriginPoint& origin = {0, 0}); // {0, 0} là góc trái màn hình
		} camera;
		bool visible = true;

		virtual ~Layer() = default;
		explicit Layer(const SDL_FPoint& origin_pos_in_percent = {0, 0});
		void render() const;
		void reset(bool to_initial_state = false);
	};
}