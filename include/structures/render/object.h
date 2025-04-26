#pragma once
#include <memory>
#include <variant>
#include "structures/render/texture.h"
#include "structures/type.hpp"

namespace Structures::Render::Object
{
	using namespace Structures::Render::Texture;
	constexpr uint8_t SDL_MAX_ALPHA = 255;

	struct Object
	{
		// Không còn cho tự render toàn màn hình nữa (dstrect = nullptr);
		// nếu muốn thì set config.render_pos về {0, 0}, set config.origin_point_in_percent về góc trái, rồi chỉnh set_render_size là logical character_render_size của màn hình
		struct Config
		{
			struct OriginPoint : SDL_FPoint
			{
				[[nodiscard]] static OriginPoint translate_origin_type_to_point(const Types::Render::OriginType& origin_type, const SDL_FPoint& size);
				[[nodiscard]] SDL_FPoint convert_pos_from_origin(const SDL_FPoint& pos, const OriginPoint& from_origin = { 0, 0 }) const;
				[[nodiscard]] SDL_FPoint convert_pos_to_origin(const SDL_FPoint& pos, const OriginPoint& to_origin = { 0, 0 }) const;
				[[nodiscard]] SDL_FRect convert_rect_from_origin(const SDL_FRect& rect, const OriginPoint& to_origin = { 0, 0 }) const;
				[[nodiscard]] SDL_FRect convert_rect_to_origin(const SDL_FRect& rect, const OriginPoint& to_origin = { 0, 0 }) const;

				OriginPoint() = default;
				OriginPoint(const float x, const float y) : SDL_FPoint{.x = x, .y = y } {}
				OriginPoint(const SDL_FPoint& point) : SDL_FPoint(point) {}
				explicit OriginPoint(const Types::Render::OriginType& origin_type, const SDL_FPoint& size);
			};
		protected:
			OriginPoint origin_point = { 0, 0 }; //! QUY ƯỚC THEO SRC, SỬ DỤNG get/set_origin_point() thay vì lấy trực tiếp!!
			SDL_FPoint scale = { 1.0f, 1.0f }; // an toàn cho set scale = 0

		public:
			SDL_FPoint render_pos; // chính là dst_rect với character_render_size nguyên gốc (muốn đổi character_render_size hãy ra ngoài Object tìm set_render_size())
			float angle = 0.0f; // theo chiều kim đồng hồ, tính bằng độ (degree), chỉ xoay hình được render ra chứ không thay đổi vị trí gốc (hitbox vẫn giữ nguyên)
			SDL_Color color = { 255, 255, 255, SDL_MAX_ALPHA }; // chỉ ảnh hưởng hình được render ra (cũng tương tự như angle)
			SDL_FlipMode flip_mode = SDL_FLIP_NONE; // chỉ ảnh hưởng hình được render ra (cũng tương tự như angle)
			SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND; // chỉ ảnh hưởng hình được render ra (cũng tương tự như angle)

			[[nodiscard]] OriginPoint get_origin_point(bool based_on_render_size = false) const;
			void set_origin_point(const SDL_FPoint& pos, bool based_on_render_size = false);
			void set_scale(const SDL_FPoint& value);
			void set_scale(const float& value);
			void set_render_size(const SDL_FPoint& size, const SDL_FPoint& src_size); // cần src_size để chia ra tỉ lệ
			void set_render_size(const float& value, const SDL_FPoint& src_size); // cần src_size để chia ra tỉ lệ
			[[nodiscard]] SDL_FRect get_sdl_dst_rect(const SDL_FPoint& src_size) const; // dst_rect trong params render của sdl

			Config();
			explicit Config(const SDL_FPoint& render_pos, const OriginPoint& origin_pos);
		};

		Memory::Item src;
		SDL_FRect src_rect_in_percent = { 0, 0, 1, 1 }; // cũng là sdl src_rect, nhưng ở % so với gốc
		Config config;
		bool visible = true;

		[[nodiscard]] SDL_FRect get_sdl_src_rect() const; // CẨN THẬN NHẦM LẪN KHI LẤY previous_render_size -> cần dùng get_sdl_dst_rect()
		[[nodiscard]] SDL_FRect get_sdl_dst_rect() const;
		[[nodiscard]] Config::OriginPoint translate_origin_type_to_point(
			const Types::Render::OriginType& origin_type, bool based_on_render_size = false) const;
		
		void set_render_size(const SDL_FPoint& size); // chính là đang set dst_rect character_render_size (muốn set pos vào trong Mapset::Object::Config tìm render_pos)
		void set_render_size(const float& value);
		void update_origin_point(const Config::OriginPoint& custom_origin, bool move_render_pos = false, bool based_on_render_size = false);
		void update_origin_point(const Types::Render::OriginType& origin_type, bool move_render_pos = false);
		virtual void render(const SDL_FPoint& offset = { 0, 0 });

		Object() = default;
		explicit Object(
			Memory::Item texture,
			const Types::Render::OriginType& origin_type = Types::Render::OriginType::Centre,
			const SDL_FPoint& render_pos = { 0, 0 });
		explicit Object(
			Memory::Item texture,
			const Config::OriginPoint& custom_origin,
			const SDL_FPoint& render_pos = { 0, 0 });
		virtual ~Object() = default;
	};

	struct Collection
	{
	private:
		void render_single(const unsigned long& index, const SDL_FPoint& total_offset) const;
		void render_in_range(const int64_t& from, const int64_t& to, const SDL_FPoint& total_offset) const;

	public:
		std::vector<std::variant<std::shared_ptr<Object>, std::shared_ptr<Collection>>> data;
		std::vector<std::pair<int64_t, int64_t>> render_range; // Phần tử đầu tiên là 0
		SDL_FPoint offset = { 0, 0 };
		bool visible = true;

		virtual void render(const SDL_FPoint& offset);

		Collection() = default;
		virtual ~Collection() = default;
	};
}