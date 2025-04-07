#pragma once
#include <memory>
#include "structures/render/textures/texture.h"

namespace Work::Render::Textures
{
	using Structures::Render::Textures::TextureMemory;

	inline std::unique_ptr<TextureMemory> skin;

	static void init_all(SDL_Renderer* renderer)
	{
		if (!skin) skin = std::make_unique<TextureMemory>(renderer);
	}
	static void clear_all()
	{
		if (skin) skin->clear();
	}
}