#pragma once
#include <string>
#include <unordered_map>
#include <SDL3/SDL_render.h>

namespace Structures::Render::Objects
{
	struct Object;
}

namespace Structures::Render::Textures
{
	struct TextureMemory : std::unordered_map<std::string, SDL_Texture*>
	{
	protected:
		using BASE = std::unordered_map<std::string, SDL_Texture*>;

	public:
		struct Item
		{
			const TextureMemory* memory = nullptr;
			const_iterator item;

		private:
			[[nodiscard]] SDL_FPoint get_size() const;

			friend Objects::Object;
		public:
			[[nodiscard]] const std::string& get_name() const;
			void change_target(const std::string& new_target = {}, const TextureMemory* new_memory = nullptr);

			Item() = default;
			Item(const std::string& name, const TextureMemory* memory);
			Item(const_iterator item_in_memory, const TextureMemory* memory);
		};

		SDL_Renderer* renderer;
		Item load_texture(const char* file_path, const std::string& name, bool override = true);
		Item load_texture(SDL_Texture* texture, const std::string& name, bool override = true);
		[[nodiscard]] static SDL_FPoint get_texture_size(const const_iterator& texture);
		[[nodiscard]] SDL_FPoint get_texture_size(const std::string& name) const;
		[[nodiscard]] Item find(const std::string& name) const;
		iterator rename_texture(const std::string& old_name, const std::string& new_name);
		iterator move_texture(const std::string& name, TextureMemory* to_memory);
		void free_texture(const std::string& name);
		void free_all();
		void clear() { free_all(); } // Tránh gọi nhầm hàm reset() của std::unordered_map

		explicit TextureMemory(SDL_Renderer* renderer) : renderer(renderer) {}
		~TextureMemory() { free_all(); }
	};
}