#pragma once
#include <memory>
#include "structures/render/textures/texture.h"
#include "structures/types.h"

namespace Structures::Render::Objects
{
	using namespace Structures::Render::Textures;
	constexpr uint8_t SDL_MAX_ALPHA = 255;

	struct Object
	{
		// Không còn cho tự render toàn màn hình nữa (dstrect = nullptr);
		// nếu muốn thì set config.render_pos về {0, 0}, set config.origin_point về góc trái, rồi chỉnh set_render_size là logical size của màn hình
		struct Config
		{
			struct OriginPoint : SDL_FPoint
			{
				[[nodiscard]] static OriginPoint translate_origin_type_to_point(const SDL_FPoint& size, const Types::Render::RenderOriginType& origin_type);
				[[nodiscard]] SDL_FPoint convert_pos_from_origin(const SDL_FPoint& pos, const OriginPoint& from_origin = { 0, 0 }) const;
				[[nodiscard]] SDL_FPoint convert_pos_to_origin(const SDL_FPoint& pos, const OriginPoint& to_origin = { 0, 0 }) const;
				[[nodiscard]] SDL_FRect convert_rect_from_origin(const SDL_FRect& rect, const OriginPoint& to_origin = { 0, 0 }) const;
				[[nodiscard]] SDL_FRect convert_rect_to_origin(const SDL_FRect& rect, const OriginPoint& to_origin = { 0, 0 }) const;
			};
		protected:
			OriginPoint origin_point = { 0, 0 }; //! QUY ƯỚC THEO SRC, SỬ DỤNG get/set_origin_point() thay vì lấy trực tiếp!!
			SDL_FPoint scale = { 1.0f, 1.0f }; // an toàn cho set scale = 0

		public:
			SDL_FPoint render_pos; // chính là dst_rect với size nguyên gốc (muốn đổi size hãy ra ngoài Object tìm set_render_size())
			float angle = 0.0f; // theo chiều kim đồng hồ, tính bằng độ (degree), chỉ xoay hình được render ra chứ không thay đổi vị trí gốc (hitbox vẫn giữ nguyên)
			SDL_Color color = { 255, 255, 255, SDL_MAX_ALPHA }; // chỉ ảnh hưởng hình được render ra (cũng tương tự như angle)
			SDL_FlipMode flip_mode = SDL_FLIP_NONE; // chỉ ảnh hưởng hình được render ra (cũng tương tự như angle)
			SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND; // chỉ ảnh hưởng hình được render ra (cũng tương tự như angle)

			[[nodiscard]] OriginPoint get_origin_point(bool based_on_render_size = false) const;
			void set_origin_point(const SDL_FPoint& pos, bool based_on_render_size = false);
			void set_scale(const SDL_FPoint& value);
			void set_scale(const float& value);
			void set_fixed_render_size(const SDL_FPoint& size, const SDL_FPoint& src_size); // cần src_size để chia ra tỉ lệ
			void set_fixed_render_size(const float& value, const SDL_FPoint& src_size); // cần src_size để chia ra tỉ lệ
			[[nodiscard]] SDL_FRect get_sdl_dst_rect(const SDL_FPoint& src_size) const; // dst_rect trong params render của sdl

			Config();
			explicit Config(const SDL_FPoint& render_pos, const OriginPoint& origin_pos);
		};

		TextureMemory::Item src;
		SDL_FRect src_rect_in_percent = { 0, 0, 1, 1 }; // cũng là sdl src_rect, nhưng ở % so với gốc
		Config config;
		bool visible = true;

		[[nodiscard]] SDL_FRect get_sdl_src_rect() const; // CẨN THẬN NHẦM LẪN KHI LẤY previous_render_size -> cần dùng get_sdl_dst_rect()
		[[nodiscard]] SDL_FRect get_sdl_dst_rect() const;
		[[nodiscard]] Config::OriginPoint translate_origin_type_to_point(
			const Types::Render::RenderOriginType& origin_type, bool based_on_render_size = false) const;
		
		void set_render_size(const SDL_FPoint& size); // chính là đang set dst_rect size (muốn set pos vào trong Collection::Object::Config tìm render_pos)
		void set_render_size(const float& value);
		void update_origin_point(const Config::OriginPoint& custom_origin, bool move_render_pos = false, bool based_on_render_size = false);
		void update_origin_point(const Types::Render::RenderOriginType& origin_type, bool move_render_pos = false);
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
	using PolyObjectUnique = std::unique_ptr<PolyObject>;
}