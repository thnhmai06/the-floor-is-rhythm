#pragma once
#include <stb_image.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include "logging.h"

struct TextureMemory : std::unordered_map<std::string, SDL_Texture*>
{
	SDL_Renderer* renderer;
	int32_t desired_channels = STBI_rgb_alpha;

	SDL_Texture* load(const char* file_path, const std::string& name);
	SDL_Texture* load(SDL_Texture* texture, const std::string& name);
	void render(const std::string& name, const SDL_FRect* rect) const;
	void free(const std::string& name);
	void free_all();

	explicit TextureMemory(SDL_Renderer* renderer) : renderer(renderer) {}
};