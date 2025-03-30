#include "render/texture.h" // Header
#include <ranges>
#include "logging.h"
#include "exceptions.h"
#include "utilities.h"

//! Texture
// ::TextureMemory
SDL_Texture* Texture::TextureMemory::load(const char* file_path, const std::string& name)
{
	SDL_Texture* texture = IMG_LoadTexture(this->renderer, file_path);
	if (!texture)
		THROW_ERROR(SDLExceptions::Texture::SDL_Texture_Load_Failed(file_path));

	// Nếu dùng STB_Image
	/*if (!texture)
		THROW_ERROR(SDLExceptions::Texture::IMG_LoadTexture_Failed(file_path));

	int32_t w, h, channels;
	unsigned char* image = stbi_load(file_path, &w, &h, &channels, desired_channels);
	if (!image)
		THROW_ERROR(STBI_Exceptions::Image::STBI_LoadImage_Failed(file_path));
	SDL_Texture* texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w, h);

	if (!SDL_UpdateTexture(texture, nullptr, image, w * desired_channels))
	{
		stbi_image_free(image);
		THROW_ERROR(SDLExceptions::Texture::SDL_UpdateTexture_Failed(file_path));
	}
	stbi_image_free(image);*/

	return load(texture, name);
}
SDL_Texture* Texture::TextureMemory::load(SDL_Texture* texture, const std::string& name)
{
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	insert_or_assign(name, texture);
	return texture;
}
SDL_FPoint Texture::TextureMemory::get_texture_size(const const_iterator& texture)
{
	float width, height;
	SDL_GetTextureSize(texture->second, &width, &height);
	return { width, height };
}
SDL_FPoint Texture::TextureMemory::get_texture_size(const std::string& name) const
{
	if (const auto it = find(name); it != end())
		return get_texture_size(it);
	return { 0, 0 };
}
void Texture::TextureMemory::free(const std::string& name)
{
	if (const auto it = find(name); it != end())
	{
		SDL_DestroyTexture(it->second);
		erase(it);
	}
}
void Texture::TextureMemory::free_all()
{
	for (const auto& texture : *this | std::views::values)
		SDL_DestroyTexture(texture);
	std::unordered_map<std::string, SDL_Texture*>::clear();
}

//! RenderObjects
// ::RenderObject
SDL_FPoint RenderObjects::RenderObject::RenderOrigin::to_sdl_pos(const SDL_FPoint& texture_pos) const
{
	return { texture_pos.x + x, texture_pos.y + y};
}
SDL_FPoint RenderObjects::RenderObject::RenderOrigin::from_sdl_pos(const SDL_FPoint& sdl_pos) const
{
	return { sdl_pos.x - x, sdl_pos.y - y };
}
SDL_FRect RenderObjects::RenderObject::to_sdl_dst() const
{
	const auto [src_w, src_h] = memory->get_texture_size(*name);
	const auto [x, y] = object_origin.to_sdl_pos(render_pos);
	return { x, y, src_w * scale.x, src_h * scale.y };
}
SDL_FRect RenderObjects::RenderObject::from_sdl_dst(const SDL_FPoint& sdl_pos) const
{
	const auto [src_w, src_h] = memory->get_texture_size(*name);
	const auto [x, y] = object_origin.from_sdl_pos(sdl_pos);
	return { x, y, src_w / scale.x, src_h / scale.y };
}
void RenderObjects::RenderObject::set_scale_fixed(const SDL_FPoint& dst_size)
{
	scale.x = dst_size.x / memory->get_texture_size(*name).x;
	scale.y = dst_size.y / memory->get_texture_size(*name).y;
}

//! RenderObjects
SDL_FRect RenderObjects::RenderObject::get_src_rect_fixed() const
{
	const auto [src_x, src_y] = memory->get_texture_size(*name);
	return { src_x * src_rect_percent.x, src_y * src_rect_percent.y, src_x * src_rect_percent.w, src_y * src_rect_percent.h };
}
void RenderObjects::RenderObject::render() const
{
	const auto item = memory->find(*name);
	if (item == memory->end())
		LOG_ERROR(SDLExceptions::Texture::SDL_Texture_Render_Failed(*name));

	const auto texture = item->second;
	const auto src_rect = get_src_rect_fixed();
	const auto dst_rect = to_sdl_dst();
	SDL_SetTextureAlphaMod(texture, alpha);
	if (!SDL_RenderTexture(memory->renderer, texture, &src_rect, &dst_rect))
		LOG_ERROR(SDLExceptions::Texture::SDL_Texture_Render_Failed(*name));
}