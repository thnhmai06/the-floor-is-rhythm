#include "render/layer.h" // Header

//! Camera
void Layers::Layer::Camera::move_into_camera(Texture::TextureConfig& object) const
{
	if (object.origin_dst)
	{
		object.origin_dst->w *= zoom;
		object.origin_dst->h *= zoom;
		const auto camera_size = SDL_FPoint{
			static_cast<float>(ImmutableConfig::Video::LOGICAL_WIDTH) / zoom,
			static_cast<float>(ImmutableConfig::Video::LOGICAL_HEIGHT) / zoom
		};
		const auto camera_pos_on_layer = SDL_FPoint{
			centre_pos.x - camera_size.x / 2,
			centre_pos.y - camera_size.y / 2
		};
		object.origin_dst->x -= camera_pos_on_layer.x;
		object.origin_dst->y -= camera_pos_on_layer.y;
	}
}
void Layers::Layer::Camera::move_out_camera(Texture::TextureConfig& object) const
{
	if (object.origin_dst)
	{
		object.origin_dst->w /= zoom;
		object.origin_dst->h /= zoom;
		const auto camera_size = SDL_FPoint{
			static_cast<float>(ImmutableConfig::Video::LOGICAL_WIDTH) / zoom,
			static_cast<float>(ImmutableConfig::Video::LOGICAL_HEIGHT) / zoom
		};
		const auto camera_pos_on_layer = SDL_FPoint{
			centre_pos.x - camera_size.x / 2,
			centre_pos.y - camera_size.y / 2
		};
		object.origin_dst->x += camera_pos_on_layer.x;
		object.origin_dst->y += camera_pos_on_layer.y;
	}
}

//! Layer
void Layers::Layer::to_absolute_object(Texture::TextureConfig& object) const
{
	// dstrect
	if (config.dst_rect && object.origin_dst)
	{
		// Position
		object.origin_dst->x += config.dst_rect->x;
		object.origin_dst->y += config.dst_rect->y;

		// Scaling
		object.origin_dst->w *= config.dst_rect->w / ImmutableConfig::Video::LOGICAL_WIDTH;
		object.origin_dst->h *= config.dst_rect->h / ImmutableConfig::Video::LOGICAL_HEIGHT;
	}
	// alpha
	object.alpha = (object.alpha * config.alpha) / 255;
}
void Layers::Layer::to_relative_object(Texture::TextureConfig& object) const
{
	// dstrect
	if (config.dst_rect && object.origin_dst)
	{
		// Position
		object.origin_dst->x -= config.dst_rect->x;
		object.origin_dst->y -= config.dst_rect->y;
		// Scaling
		object.origin_dst->w *= static_cast<float>(ImmutableConfig::Video::LOGICAL_WIDTH) / config.dst_rect->w;
		object.origin_dst->h *= static_cast<float>(ImmutableConfig::Video::LOGICAL_HEIGHT) / config.dst_rect->h;
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