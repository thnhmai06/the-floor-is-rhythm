#pragma once
#include <string>
#include <unordered_map>
#include <SDL3/SDL_render.h>

struct TextureMemory : std::unordered_map<std::string, SDL_Texture*>
{
	SDL_Renderer* renderer;
	SDL_Texture* load(const char* file_path, const std::string& name);
	SDL_Texture* load(SDL_Texture* texture, const std::string& name);
	[[nodiscard]] static SDL_FPoint get_texture_size(const const_iterator& texture);
	[[nodiscard]] SDL_FPoint get_texture_size(const std::string& name) const;
	void free_texture(const std::string& name);
	void free_all();
	void clear() { free_all(); } // Tránh gọi nhầm hàm clear() của std::unordered_map

	explicit TextureMemory(SDL_Renderer* renderer) : renderer(renderer) {}
	~TextureMemory() { free_all(); }
};

struct TextureManager
{
	std::string name;
	TextureMemory* memory = nullptr;

	[[nodiscard]] SDL_FPoint get_size() const;
	void rename(const std::string& new_name) const;
	void move(TextureMemory* to_memory);
	void free();

	TextureManager() = default;
	TextureManager(std::string name, TextureMemory* memory) : name(std::move(name)), memory(memory) {}
	TextureManager(const TextureMemory::iterator& memory_item, TextureMemory* memory);
};