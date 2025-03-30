#pragma once
#include <string>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include "template.h"
#include "utilities.h"

namespace Texture
{
	struct TextureMemory : std::unordered_map<std::string, SDL_Texture*>
	{
		SDL_Renderer* renderer;
		SDL_Texture* load(const char* file_path, const std::string& name);
		SDL_Texture* load(SDL_Texture* texture, const std::string& name);
		[[nodiscard]] static SDL_FPoint get_texture_size(const const_iterator& texture);
		[[nodiscard]] SDL_FPoint get_texture_size(const std::string& name) const;
		void free(const std::string& name);
		void free_all();
		void clear() { free_all(); } // Tránh gọi nhầm hàm clear() của std::unordered_map

		explicit TextureMemory(SDL_Renderer* renderer) : renderer(renderer) {}
		~TextureMemory() { free_all(); }
	};
}

namespace RenderObjects
{
	struct RenderObject
	{
		struct RenderOrigin : SDL_FPoint
		{
			[[nodiscard]] SDL_FPoint to_sdl_pos(const SDL_FPoint& texture_pos) const; // SDL
			[[nodiscard]] SDL_FPoint from_sdl_pos(const SDL_FPoint& sdl_pos) const; // current
		};

		//? Nguồn
		const std::string* name;
		const Texture::TextureMemory* memory;
		SDL_FRect src_rect_percent = {0, 0, 1, 1};
		//? Đích
		RenderOrigin render_origin = Template::Video::TextureOrigin::TOP_LEFT;
		SDL_FPoint render_pos = { 0, 0 };
		SDL_FPoint scale = { 1.0f, 1.0f };
		uint8_t alpha = 255;

		void set_both_scale(const float& value) { scale = { .x= value, .y= value }; }
		void set_scale_fixed(const SDL_FPoint& dst_size);
		void set_scale_fixed(const float& value) { set_scale_fixed({ value, value }); }
		[[nodiscard]] SDL_FRect get_src_rect_fixed() const;
		[[nodiscard]] SDL_FRect to_sdl_dst() const;
		[[nodiscard]] SDL_FRect from_sdl_dst() const;

		virtual void render() const;
		explicit RenderObject(const std::string* name, const Texture::TextureMemory* memory, const SDL_FPoint& pos) :
			name(name), memory(memory), render_pos(pos) {
		}
		virtual ~RenderObject() = default;
	};
}