#pragma once
#include <memory>
#include "structures/render/textures/texture.h"
#include "structures/types.h"

namespace Structures::Render::Objects
{
	using namespace Structures::Render::Textures;

	struct Object
	{
		// Không còn cho tự render toàn màn hình nữa (dstrect = nullptr);
		// nếu muốn thì set config.render_pos về {0, 0}, set config.origin_point về góc trái, rồi chỉnh set_render_size là logical size của màn hình
		struct Config
		{
			struct OriginPoint : SDL_FPoint
			{
				[[nodiscard]] static OriginPoint get_origin_point_from_type(const TextureMemory::Item& src, const Types::Render::RenderOriginType& origin_type);
				[[nodiscard]] SDL_FPoint convert_pos_from_origin(const SDL_FPoint& pos, const OriginPoint& from_origin = { 0, 0 }) const;
				[[nodiscard]] SDL_FPoint convert_pos_to_origin(const SDL_FPoint& pos, const OriginPoint& to_origin = { 0, 0 }) const;
				[[nodiscard]] SDL_FRect convert_rect_from_origin(const SDL_FRect& rect, const OriginPoint& to_origin = { 0, 0 }) const;
				[[nodiscard]] SDL_FRect convert_rect_to_origin(const SDL_FRect& rect, const OriginPoint& to_origin = { 0, 0 }) const;
			};
			SDL_FPoint render_pos = { 0, 0 }; // chính là dst_rect với size nguyên gốc (muốn đổi size hãy ra ngoài Object tìm set_render_size())
			OriginPoint origin_point = { 0, 0 }; // góc trái texture
			SDL_FPoint scale = { 1.0f, 1.0f };
			uint8_t alpha = 255;

			void set_both_scale(const float& value);
			void set_scale_fixed(const SDL_FPoint& size, const SDL_FPoint& src_size);
			void set_scale_fixed(const float& value, const SDL_FPoint& src_size);
			[[nodiscard]] SDL_FRect get_sdl_dst_rect(const SDL_FPoint& src_size) const; // dst_rect trong params render của sdl

			Config();
			explicit Config(const SDL_FPoint& render_pos, const OriginPoint& origin = { 0.0f, 0.0f });
		};

		TextureMemory::Item src;
		SDL_FRect src_rect_in_percent = { 0, 0, 1, 1 }; // cũng là sdl src_rect, nhưng ở % so với gốc
		Config config;
		bool visible = true;

	protected:
		[[nodiscard]] SDL_FRect get_sdl_src_rect() const;
		[[nodiscard]] SDL_FRect get_sdl_dst_rect() const;

	public:
		[[nodiscard]] SDL_FPoint get_sdl_render_pos() const;
		[[nodiscard]] SDL_FPoint get_render_size() const;
		[[nodiscard]] Config::OriginPoint get_origin_point_from_type(const Types::Render::RenderOriginType& origin_type) const;
		void set_render_size(const SDL_FPoint& size); // chính là đang set dst_rect size (muốn set pos vào trong Collection::Object::Config tìm render_pos)
		void set_render_size(const float& value);
		void set_render_size_based_on_src(const SDL_FPoint& src_percent);
		void set_origin_point(const Types::Render::RenderOriginType& origin_type);
		void set_origin_point(const Config::OriginPoint& custom_origin);
		void render(const SDL_FPoint& offset = { 0, 0 }) const;

		Object() = default;
		explicit Object(
			TextureMemory::Item texture,
			const Types::Render::RenderOriginType& origin_type = Types::Render::RenderOriginType::CENTRE,
			const SDL_FPoint& render_pos = { 0, 0 });
		explicit Object(
			TextureMemory::Item texture,
			const Config::OriginPoint& custom_origin,
			const SDL_FPoint& render_pos = { 0, 0 });
		virtual ~Object() = default;
	};
	struct PolyObject
	{
	protected:
		using CONTAINER = std::vector<Object>;

	public:
		CONTAINER data;
		bool visible = true;
		void render(const SDL_FPoint& offset = { 0, 0 }) const;

		auto operator+=(const CONTAINER& other);
		auto operator+=(const Object& obj);
		virtual ~PolyObject() = default;
	};

	using ObjectUnique = std::unique_ptr<Object>;
	using PolyObjectUnique = std::unique_ptr<PolyObject>;;
}