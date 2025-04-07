#pragma once
#include <string>
#include <unordered_map>
#include <SDL3/SDL_render.h>

namespace Structures::Render::Textures
{
	struct TexturePtr;

	struct TextureMemory : std::unordered_map<std::string, SDL_Texture*>
	{
	protected:
		using BASE = std::unordered_map<std::string, SDL_Texture*>;

	public:
		SDL_Renderer* renderer;
		TexturePtr load_texture(const char* file_path, const std::string& name, bool override = true);
		TexturePtr load_texture(SDL_Texture* texture, const std::string& name, bool override = true);
		[[nodiscard]] static SDL_FPoint get_texture_size(const const_iterator& texture);
		[[nodiscard]] SDL_FPoint get_texture_size(const std::string& name) const;
		[[nodiscard]] TexturePtr find(const std::string& name) const;
		iterator rename_texture(const std::string& old_name, const std::string& new_name);
		iterator move_texture(const std::string& name, TextureMemory* to_memory);
		void free_texture(const std::string& name);
		void free_all();
		void clear() { free_all(); } // Tránh gọi nhầm hàm reset() của std::unordered_map

		explicit TextureMemory(SDL_Renderer* renderer) : renderer(renderer) {}
		~TextureMemory() { free_all(); }
	};

	struct TexturePtr
	{
		TextureMemory::const_iterator item;
		const TextureMemory* memory = nullptr;

		[[nodiscard]] const std::string& get_name() const;
		[[nodiscard]] SDL_FPoint get_size() const;
		void change_target(const std::string& new_target = {}, const TextureMemory* new_memory = nullptr);

		TexturePtr() = default;
		TexturePtr(const std::string& name, const TextureMemory* memory);
		TexturePtr(TextureMemory::const_iterator item_in_memory, const TextureMemory* memory);
	};
}