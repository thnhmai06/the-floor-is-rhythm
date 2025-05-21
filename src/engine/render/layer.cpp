#include "engine/render/layer.h" // Header

#include "logging/exceptions.h"
#include "logging/logger.h"

using namespace Utilities::Math::FPoint;

namespace Engine::Render::Layer
{
	//! Layer
	void Layer::put_object_into_layer(Config::ObjectConfig& object) const {}
	void Layer::put_object_out_layer(Config::ObjectConfig& object) const {}
	void Layer::render_object(Object::Object& object, const Config::RenderConfig& parent)
	{
		if (!parent.is_visible()) return;
		if (!object.config.is_visible() || !object.src.is_valid()) return;

		object.on_before_render();
		put_object_into_layer(object.config);
		const auto& sdl_texture = object.src.item->second;
		const auto src_rect = object.get_sdl_src_rect();
		auto dst_rect = object.get_sdl_dst_rect();
		dst_rect.x -= parent.render_pos.x; dst_rect.y -= parent.render_pos.y;
		const uint8_t final_alpha = object.config.alpha.get_value(true, object.config.alpha.percent * parent.alpha.percent);

		// https://stackoverflow.com/questions/24969783/is-it-safe-acceptable-to-call-sdl-settexturecolormod-every-frame-multiple-times
		if (!SDL_SetTextureBlendMode(sdl_texture, object.config.blend_mode))
			LOG_ERROR(Logging::Exceptions::Engine::Render::SDL_Render_TextureSetBlendMode_Failed(object.src.get_name(), object.config.blend_mode));
		if (!SDL_SetTextureAlphaMod(sdl_texture, final_alpha))
			LOG_ERROR(Logging::Exceptions::Engine::Render::SDL_Render_TextureSetAlphaMod_Failed(object.src.get_name(), object.config.alpha.percent));
		if (!SDL_SetTextureColorMod(sdl_texture, object.config.color.r, object.config.color.g, object.config.color.b))
			LOG_ERROR(Logging::Exceptions::Engine::Render::SDL_Render_TextureSetColor_Failed(object.src.get_name(), object.config.color));
		const SDL_FPoint anchor_point = object.config.get_origin_point(true);
		if (!SDL_RenderTextureRotated(object.src.memory->renderer, sdl_texture, &src_rect,
			&dst_rect, object.config.angle, &anchor_point, object.config.flip_mode.get_mode()))
			LOG_ERROR(Logging::Exceptions::Engine::Render::SDL_Render_RenderTexture_Failed(object.src.get_name()));

		put_object_out_layer(object.config);
		object.on_after_render();
	}
	void Layer::render_collection(Object::Collection& collection, const Config::RenderConfig& parent)
	{
		if (!parent.is_visible()) return;
		if (!collection.config.is_visible()) return;

		collection.on_before_render();
		collection.config *= parent;

		collection.for_each_item([&](Object::Collection::Container::reference& item) -> bool
			{
				if (std::holds_alternative<std::shared_ptr<Object::Object>>(item))
				{
					const auto& object = std::get<std::shared_ptr<Object::Object>>(item);
					render_object(*object, collection.config);
				}
				else if (std::holds_alternative<std::shared_ptr<Object::Collection>>(item))
				{
					const auto& collection_ = std::get<std::shared_ptr<Object::Collection>>(item);
					render_collection(*collection_, collection.config);
				}
				return true;
			}, true);

		collection.config /= parent;
		collection.on_after_render();
	}
	void Layer::render()
	{
		if (!config->visible) return;
		buffer.for_each_item(
			[&](const std::shared_ptr<Object::Object>& item) { render_object(*item, *config); },
			[&](const std::shared_ptr<Object::Collection>& item) { render_collection(*item, *config); });
	}
	void Layer::clear()
	{
		buffer.data.clear();
	}
	Layer::Layer(const SDL_FPoint& camera_centre_pos)
	{
		config = std::make_unique<Config::RenderConfig>(camera_centre_pos);
	}

	//! TextureLayer
	void TextureLayer::put_object_into_layer(Config::ObjectConfig& object) const
	{
		const auto pixel = target_texture.get_size() / grid_size.value();
		object.render_pos *= pixel;
		object.scale *= pixel;
	}
	void TextureLayer::put_object_out_layer(Config::ObjectConfig& object) const
	{
		const auto pixel = target_texture.get_size() / grid_size.value();
		object.render_pos /= pixel;
		object.scale /= pixel;
	}
	void TextureLayer::render_no_change_back_target(const bool clear)
	{
		if (!config->is_visible() || buffer.data.empty()) return;

		SDL_SetRenderTarget(target_texture.memory->renderer, target_texture.item->second);
		if (clear) SDL_RenderClear(target_texture.memory->renderer);
		Layer::render();
	}
	void TextureLayer::render()
	{
		if (!config->is_visible() || buffer.data.empty()) return;

		SDL_Texture* const current_target = SDL_GetRenderTarget(target_texture.memory->renderer);
		render_no_change_back_target();
		SDL_SetRenderTarget(target_texture.memory->renderer, current_target);
	}
	void TextureLayer::clear()
	{
		Layer::clear();
		target_texture.clear();
	}
	TextureLayer::TextureLayer(Memory::Item texture,
		const SDL_FPoint& centre_camera_pos)
		: Layer(centre_camera_pos), target_texture(std::move(texture))
	{
	}
}