#pragma once
#include <SDL3_image/SDL_image.h>
#include <unordered_map>
#include "logging.h"

struct TextureRenderConfig
{
	SDL_FRect* src_rect = nullptr;
	SDL_FRect* dst_rect = nullptr;
	uint8_t alpha = 255;

	struct Rotation
	{
		double_t angle = 0;
		SDL_FPoint* center = nullptr;
		SDL_FlipMode flip = SDL_FLIP_NONE;
	};
	std::optional<Rotation> rotation;
	TextureRenderConfig(SDL_FRect* dst = nullptr, uint8_t alpha = 255) : dst_rect(dst), alpha(alpha) {}
};

struct TextureMemory : std::unordered_map<std::string, SDL_Texture*>
{
	SDL_Renderer* renderer;
	SDL_Texture* load(const char* file_path, const std::string& name);
	SDL_Texture* load(SDL_Texture* texture, const std::string& name);
	bool render(const std::string& name, const TextureRenderConfig& config) const;
	void free(const std::string& name);
	void free_all();
	void clear() { free_all(); } // Tránh gọi nhầm hàm clear() của std::unordered_map
	explicit TextureMemory(SDL_Renderer* renderer) : renderer(renderer) {}
};