#pragma once
#include <string>
#include <unordered_map>
#include <SDL3/SDL_render.h>

struct Texture;

struct TextureMemory : std::unordered_map<std::string, SDL_Texture*>
{
protected:
	using BASE = std::unordered_map<std::string, SDL_Texture*>;

public:
	SDL_Renderer* renderer;
	SDL_Texture* load_texture(const char* file_path, const std::string& name);
	SDL_Texture* load_texture(SDL_Texture* texture, const std::string& name);
	[[nodiscard]] static SDL_FPoint get_texture_size(const const_iterator& texture);
	[[nodiscard]] static SDL_FPoint get_texture_size(const std::string& name);
	Texture find(const std::string& name);
	iterator rename_texture(const std::string& old_name, const std::string& new_name);
	iterator move_texture(const std::string& name, TextureMemory* to_memory);
	void free_texture(const std::string& name);
	void free_all();
	void clear() { free_all(); } // Tránh gọi nhầm hàm clear() của std::unordered_map

	Texture operator[](const std::string& name);
	explicit TextureMemory(SDL_Renderer* renderer) : renderer(renderer) {}
	~TextureMemory() { free_all(); }
};

struct Texture
{
	TextureMemory::const_iterator item;
	TextureMemory* memory = nullptr;

	[[nodiscard]] const std::string& get_name() const;
	[[nodiscard]] SDL_FPoint get_size() const;
	void rename(const std::string& new_name);
	void move(TextureMemory* to_memory);
	void free();

	Texture() = default;
	Texture(const std::string& name, TextureMemory* memory);
	Texture(TextureMemory::const_iterator item_in_memory, TextureMemory* memory);
};