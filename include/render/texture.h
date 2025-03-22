#pragma once
#include <stb_image.h>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include "logging.h"

struct TextureRenderConfig
{
	SDL_FRect* src_rect = nullptr;
	SDL_FRect* dst_rect = nullptr;

	struct Rotation
	{
		double_t angle = 0;
		SDL_FPoint* center = nullptr;
		SDL_FlipMode flip = SDL_FLIP_NONE;
	};
	std::optional<Rotation> rotation;
};

struct TextureMemory : std::unordered_map<std::string, SDL_Texture*>
{
	SDL_Renderer* renderer;
	int32_t desired_channels = STBI_rgb_alpha;

	SDL_Texture* load(const char* file_path, const std::string& name);
	SDL_Texture* load(SDL_Texture* texture, const std::string& name);
	bool render(const std::string& name, const TextureRenderConfig& config) const;
	void free(const std::string& name);
	void free_all();

	explicit TextureMemory(SDL_Renderer* renderer) : renderer(renderer) {}

	
};