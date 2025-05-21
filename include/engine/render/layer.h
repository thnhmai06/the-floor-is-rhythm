#pragma once
#include "engine/render/object.h"

namespace Engine::Render::Layer
{
	using namespace Render::Texture;

	struct Layer
	{
		Object::Buffer buffer;
		std::unique_ptr<Config::RenderConfig> config; // render pos đóng vai trò là camera centre

	protected:
		virtual void put_object_into_layer(Config::ObjectConfig& object) const;
		virtual void put_object_out_layer(Config::ObjectConfig& object) const;
		virtual void render_object(Object::Object& object, const Config::RenderConfig& parent = {});
		virtual void render_collection(Object::Collection& collection, const Config::RenderConfig& parent = {});

	public:
		virtual void render();
		virtual void clear();

		explicit Layer(const SDL_FPoint& camera_centre_pos = {0, 0});
		virtual ~Layer() = default;
	};

	//! Chú ý: Khi dùng TextureLayer cần chú ý đến việc
	//! cộng màu (ADD Blend mode) vì nó phụ thuộc texture trước nó
	struct TextureLayer : Layer
	{
	protected:
		void put_object_into_layer(Config::ObjectConfig& object) const override;
		void put_object_out_layer(Config::ObjectConfig& object) const override;

	public:
		Memory::Item target_texture;
		// Grid sẽ mô phỏng việc đặt một lưới grid_size đều nhau lên texture, khi
		// đó tọa độ vị trí on_before_render sẽ tương ứng đặt lên tọa độ trên lưới đó
		std::optional<SDL_FPoint> grid_size = std::nullopt;

		void render_no_change_back_target(bool clear = false);
		void render() override;
		void clear() override;

		explicit TextureLayer(Memory::Item texture,
			const SDL_FPoint& centre_camera_pos = { 0, 0 });
	};
}
