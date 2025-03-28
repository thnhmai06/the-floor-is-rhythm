#include "render/layer.h" // Header

//! Camera
void Layers::Layer::Camera::move_into_camera(Texture::TextureConfig& object) const
{
	if (object.dst_rect)
	{
		object.dst_rect->w *= zoom;
		object.dst_rect->h *= zoom;
		const auto camera_size = SDL_FPoint{
			static_cast<float>(ImmutableConfig::Video::LOGICAL_WIDTH) / zoom,
			static_cast<float>(ImmutableConfig::Video::LOGICAL_HEIGHT) / zoom
		};
		const auto camera_pos_on_layer = SDL_FPoint{
			centre_pos.x - camera_size.x / 2,
			centre_pos.y - camera_size.y / 2
		};
		object.dst_rect->x -= camera_pos_on_layer.x;
		object.dst_rect->y -= camera_pos_on_layer.y;
	}
}
void Layers::Layer::Camera::move_out_camera(Texture::TextureConfig& object) const
{
	if (object.dst_rect)
	{
		object.dst_rect->w /= zoom;
		object.dst_rect->h /= zoom;
		const auto camera_size = SDL_FPoint{
			static_cast<float>(ImmutableConfig::Video::LOGICAL_WIDTH) / zoom,
			static_cast<float>(ImmutableConfig::Video::LOGICAL_HEIGHT) / zoom
		};
		const auto camera_pos_on_layer = SDL_FPoint{
			centre_pos.x - camera_size.x / 2,
			centre_pos.y - camera_size.y / 2
		};
		object.dst_rect->x += camera_pos_on_layer.x;
		object.dst_rect->y += camera_pos_on_layer.y;
	}
}

//! Layer
void Layers::Layer::to_absolute_object(Texture::TextureConfig& object) const
{
	// dstrect
	if (config.dst_rect && object.dst_rect)
	{
		// Position
		object.dst_rect->x += config.dst_rect->x;
		object.dst_rect->y += config.dst_rect->y;

		// Scaling
		object.dst_rect->w *= config.dst_rect->w / ImmutableConfig::Video::LOGICAL_WIDTH;
		object.dst_rect->h *= config.dst_rect->h / ImmutableConfig::Video::LOGICAL_HEIGHT;
	}
	// alpha
	object.alpha = (object.alpha * config.alpha) / 255;
}
void Layers::Layer::to_relative_object(Texture::TextureConfig& object) const
{
	// dstrect
	if (config.dst_rect && object.dst_rect)
	{
		// Position
		object.dst_rect->x -= config.dst_rect->x;
		object.dst_rect->y -= config.dst_rect->y;
		// Scaling
		object.dst_rect->w *= static_cast<float>(ImmutableConfig::Video::LOGICAL_WIDTH) / config.dst_rect->w;
		object.dst_rect->h *= static_cast<float>(ImmutableConfig::Video::LOGICAL_HEIGHT) / config.dst_rect->h;
	}
	// alpha
	object.alpha = (object.alpha * 255) / config.alpha;
}
void Layers::Layer::render()
{
	if (!visible) return;

	const auto begin = render_range ? render_range->first : render_buffer.begin();
	const auto end = render_range ? render_range->second : render_buffer.end();
	for (auto object = begin; object != end; ++object)
	{
		// i hate this job
		to_absolute_object(object->config);
		camera.move_into_camera(object->config);
		object->render(memory);
		camera.move_out_camera(object->config);
		to_relative_object(object->config);
	}
}

Layers::Layer::Layer(SDL_Renderer* renderer) : renderer(renderer), memory(renderer) {}
void Layers::Layer::clear(const bool to_initial_state)
{
	render_buffer.clear();
	if (to_initial_state)
	{
		config = LayerConfig();
		memory.free_all();
	}
}