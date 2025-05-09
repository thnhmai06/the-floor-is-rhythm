// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
#pragma once
#include "structures/render/object.h"
#include "utilities.hpp"

namespace Structures::Render::Layer
{
	using namespace Render::Texture;

	struct Layer
	{
		struct Buffer
		{
			using CONTAINER = std::list<std::weak_ptr<Object::Collection>>;

			const Layer* parent;
			CONTAINER data;

			struct Item
			{
				Buffer* parent;
				CONTAINER::iterator item;

				void destroy();

				explicit Item(Buffer* render_buffer);
				Item(Buffer* render_buffer, CONTAINER::iterator item);
			};

			Item add(const std::weak_ptr<Object::Collection>& collection);
			static void destroy(Item& item);
			void for_each_item(const std::function<void(std::shared_ptr<Object::Collection>& item)>& function);

			explicit Buffer(const Layer* layer) : parent(layer) {}
		};
		struct Camera : private Object::Object::Config
		{
		protected:
			using BASE = Object::Object::Config;

		public:
			[[nodiscard]] SDL_FPoint get_camera_object_offset() const;
			[[nodiscard]] const SDL_FPoint& get_camera_origin() const;
			[[nodiscard]] SDL_FPoint& get_camera_origin();
			[[nodiscard]] SDL_FPoint& get_camera_pos();
			[[nodiscard]] const SDL_FPoint& get_camera_pos() const;
			[[nodiscard]] static SDL_FPoint get_camera_size();

			explicit Camera(const OriginPoint& origin = { 0, 0 }); // {0, 0} là góc trái màn hình
		};

		// attributes
		Buffer render_buffer;
		Camera camera;
		bool visible = true;

	protected:
		virtual void put_object_into_layer(std::shared_ptr<Object::Object>& object) {}
		virtual void put_object_out_layer(std::shared_ptr<Object::Object>& object) {}

	public:
		virtual void render_object(std::shared_ptr<Object::Object>& object, const SDL_FPoint& offset = {0, 0});
		virtual void render_collection(std::shared_ptr<Object::Collection>& collection, const SDL_FPoint& bonus_offset = {0, 0});
		virtual void render();
		virtual void reset(bool reset_camera = false);

		explicit Layer(const SDL_FPoint& camera_origin_in_percent = { 0, 0 });
		virtual ~Layer() = default;
	};

	struct TextureLayer : Layer
	{
		Memory::Item target_texture;
		// Grid sẽ mô phỏng việc đặt một lưới grid_size đều nhau lên texture, khi
		// đó tọa độ vị trí on_before_render sẽ tương ứng đặt lên tọa độ trên lưới đó
		std::optional<SDL_FPoint> grid_size = std::nullopt;

	protected:
		void put_object_into_layer(std::shared_ptr<Object::Object>& object) override;
		void put_object_out_layer(std::shared_ptr<Object::Object>& object) override;

	public:
		void render_no_change_back_target(bool clear = false);
		void render() override;
		void reset(bool reset_camera = false) override;

		explicit TextureLayer(Memory::Item texture,
			const SDL_FPoint& camera_origin_in_percent = { 0, 0 });
	};
}