#include "render/playground/r_object.h" // Header
#include "exceptions.h"
#include "rule/skin.h"

//! RenderObject
void RenderObject::RenderObject::render(const TextureMemory& memory) const
{
	if (!memory.render(*name, config))
		LOG_ERROR(SDL_Exceptions::Texture::SDL_RenderTexture_Failed(*name));
}

//! RenderFloor
RenderObject::RenderFloor::RenderFloor(const HitObject::Floor* floor, const SDL_FRect* prev_dstrect_config) : floor(floor)
{
	name = &Skin_Filename::HitObject::FLOOR;
	// tính toán trực tiếp vị trí note tiếp theo trong này.
	// nếu là note đầu tiên thì set thủ công.

}