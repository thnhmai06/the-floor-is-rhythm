#pragma once
#include <memory>
#include <optional>
#include <string>
#include <SDL3_image/SDL_image.h>
#include <unordered_map>

namespace Texture
{
	struct TextureConfig
	{
		std::unique_ptr<SDL_FRect> src_rect = nullptr;
		std::unique_ptr<SDL_FRect> dst_rect = nullptr;
		uint8_t alpha = 255;

		struct Rotation
		{
			float angle = 0.0f;
			std::unique_ptr<SDL_FPoint> center = nullptr;
			SDL_FlipMode flip = SDL_FLIP_NONE;
		};
		std::optional<Rotation> rotation = std::nullopt;
		TextureConfig(const SDL_FRect* dst = nullptr, const uint8_t alpha = 255) :
			dst_rect(dst ? std::make_unique<SDL_FRect>(*dst) : nullptr),
			alpha(alpha) {
		}
	};

	struct TextureMemory : std::unordered_map<std::string, SDL_Texture*>
	{
		SDL_Renderer* renderer;

		SDL_Texture* load(const char* file_path, const std::string& name);
		SDL_Texture* load(SDL_Texture* texture, const std::string& name);
		bool render(const std::string& name, const TextureConfig& config) const;
		void free(const std::string& name);
		void free_all();
		void clear() { free_all(); } // Tránh gọi nhầm hàm clear() của std::unordered_map
		explicit TextureMemory(SDL_Renderer* renderer) : renderer(renderer) {}
	};
}

namespace RenderObjects
{
	struct RenderObject
	{
		const std::string* name = nullptr;
		Texture::TextureConfig config;

		virtual void render(const Texture::TextureMemory& memory) const;
		virtual ~RenderObject() = default;
	};
}