#include "render/layer.h" // Header
#include "exceptions.h"
#include "config.h"
#include "logging.h"

//! RenderObject
void RenderObject::render(const TextureMemory& memory) const
{
	if (!memory.render(name, config))
		LOG_ERROR(SDL_Exceptions::Texture::SDL_RenderTexture_Failed(name));
}

//! Layer
void Layer::to_absolute_object(TextureRenderConfig& object) const
{
	// dstrect
	if (config.dst_rect)
	{
		if (object.dst_rect)
		{
			// Position
			object.dst_rect->x += config.dst_rect->x;
			object.dst_rect->y += config.dst_rect->y;

			// Scaling
			object.dst_rect->w *= config.dst_rect->w / Immutable::Video::LOGICAL_WIDTH;
			object.dst_rect->h *= config.dst_rect->h / Immutable::Video::LOGICAL_HEIGHT;
		}
	}
	// alpha
	object.alpha = (object.alpha * config.alpha) / 255;
}
void Layer::to_relative_object(TextureRenderConfig& object) const
{
	// dstrect
	if (config.dst_rect)
	{
		if (object.dst_rect)
		{
			// Position
			object.dst_rect->x -= config.dst_rect->x;
			object.dst_rect->y -= config.dst_rect->y;
			// Scaling
			object.dst_rect->w *= static_cast<float>(Immutable::Video::LOGICAL_WIDTH) / config.dst_rect->w;
			object.dst_rect->h *= static_cast<float>(Immutable::Video::LOGICAL_HEIGHT) / config.dst_rect->h;
		}
	}
	// alpha
	object.alpha = (object.alpha * 255) / config.alpha;
}
void Layer::render()
{
	for (auto& object : objects)
	{
		to_absolute_object(object.config);
		object.render(memory);
		to_relative_object(object.config);
	}
}
Layer::Layer(SDL_Renderer* renderer) : renderer(renderer), memory(renderer) {}
void Layer::clear(const bool to_initial_state)
{
	objects.clear();
	if (to_initial_state)
	{
		config = Config();
		memory.free_all();
	}
}