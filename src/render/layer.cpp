﻿#include "render/layer.h" // Header
#include "exceptions.h"
#include "config.h"

static SDL_Texture* create_layer(SDL_Renderer* renderer)
{
	return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,
		Immutable::Video::LOGICAL_WIDTH, Immutable::Video::LOGICAL_HEIGHT);
}

Layer::Layer(SDL_Renderer* renderer) : renderer(renderer), memory(renderer)
{
	layer = create_layer(renderer);
}

void Layer::free(const bool to_initial_state)
{
	objects.clear();
	SDL_DestroyTexture(layer);
	config = TextureRenderConfig();

	if (to_initial_state)
	{
		layer = create_layer(renderer);
		memory.free_all();
	}
}

bool Layer::re_draw() const
{
	// Render Objects
	SDL_SetRenderTarget(renderer, layer);
	SDL_RenderClear(renderer);
	for (const auto& [o_name, o_config] : objects)
	{
		if (!memory.render(o_name, o_config))
			LOG_ERROR(SDL_Exceptions::Texture::SDL_RenderTexture_Failed(o_name));
	}
	SDL_SetRenderTarget(renderer, nullptr);

	// Render Layer
	SDL_SetTextureAlphaMod(layer, config.alpha);
	if (config.rotation.has_value())
		return SDL_RenderTextureRotated(renderer, layer, config.src_rect, config.dst_rect,
			config.rotation->angle, config.rotation->center, config.rotation->flip);
	return SDL_RenderTexture(renderer, layer, config.src_rect, config.dst_rect);
}