#pragma once
#include <memory>
#include "structures/render/texture.h"

namespace Core::Resources
{
	namespace Textures
	{
		using Structures::Render::Texture::Memory;

		inline std::unique_ptr<Memory> skin;

		static void init_all(SDL_Renderer* renderer)
		{
			if (!skin) skin = std::make_unique<Memory>(renderer);
		}
		static void clear_all()
		{
			if (skin) skin->clear();
		}
	}
}