#pragma once
#include <functional>
#include <memory>
#include <variant>
#include "engine/render/texture.h"
#include "engine/render/config.h"

namespace Engine::Render::Layer
{
	struct Layer;
}

namespace Engine::Render::Object
{
	//? Bạn đang tìm hàm render của Object và Collection?
	// Nó đã được chuyển sang Layer rồi: link:src\engine\render\layer.cpp:void%20Layer::render_object(std::shared_ptr<Object::Object>&%20object)

	using namespace Engine::Render::Texture;
	using namespace Engine::Render::Config;

	struct Object
	{
		Memory::Item src;
		SDL_FRect src_rect_in_percent = { 0, 0, 1, 1 }; // cũng là sdl src_rect, nhưng ở % so với gốc
		ObjectConfig config;

	protected:
		virtual void on_before_render() {}
		virtual void on_after_render() {}

		friend Layer::Layer;

	public:
		[[nodiscard]] SDL_FRect get_sdl_src_rect() const; // CẨN THẬN NHẦM LẪN KHI LẤY previous_render_size -> cần dùng get_sdl_dst_rect()
		[[nodiscard]] SDL_FRect get_sdl_dst_rect() const;
		[[nodiscard]] OriginPoint translate_origin_type_to_point(
			const OriginType& origin_type, bool based_on_render_size = false) const;
		
		void set_render_size(const SDL_FPoint& size);
		void set_render_size(const float& value);
		void update_origin_point(const OriginPoint& custom_origin, bool move_render_pos = false, bool based_on_render_size = false);
		void update_origin_point(const OriginType& origin_type, bool move_render_pos = false);

		Object() = default;
		explicit Object(
			Memory::Item texture,
			const OriginType& origin_type = OriginType::Centre,
			const SDL_FPoint& render_pos = { 0, 0 });
		explicit Object(
			Memory::Item texture,
			const OriginPoint& custom_origin,
			const SDL_FPoint& render_pos = { 0, 0 });
		virtual ~Object() = default;
	};

	struct Collection
	{
		using Container = std::list<std::variant<std::shared_ptr<Object>, std::shared_ptr<Collection>>>;

		Container data;
		RenderConfig config;
		std::list<std::pair<Container::iterator, Container::iterator>> range;

	protected:
		virtual void on_before_render() {}
		virtual void on_after_render() {}

		friend Layer::Layer;

	public:
		void for_each_item(const std::function<bool(Container::reference& item)>& function, bool no_duplicate = false);

		Collection() = default;
		virtual ~Collection() = default;
	};

	struct Buffer
	{
		using Container = std::list<std::variant<std::weak_ptr<Object>, std::weak_ptr<Collection>>>;
		struct Item
		{
			Buffer* parent;
			Container::iterator item;

			void destroy();
			bool is_valid() const;

			explicit Item(Buffer* render_buffer = nullptr);
			Item(Buffer* render_buffer, Container::iterator item);
		};

		Container data;

		Item add(const std::weak_ptr<Object>& object);
		Item add(const std::weak_ptr<Collection>& collection);
		static void remove(Item& item);
		void for_each_item(
			const std::function<void(std::shared_ptr<Object>& object)>& object_function,
			const std::function<void(std::shared_ptr<Collection>& collection)>& collection_function);
	};
}