#pragma once
#include "engine/audio/memory.h"
#include "engine/render/texture.h"

namespace Core::Manager
{
	namespace System
	{
		void init();
		void quit();
	}
	namespace Config
	{
		void init();
		void quit();
	}
	namespace Window
	{
		void init();
		void quit();
	}
	namespace Renderer
	{
		void init();
		void quit();
	}
	namespace Mixer
	{
		void init();
	}
	namespace Skin
	{
		inline std::shared_ptr<Engine::Render::Texture::Memory> texture;
		inline std::shared_ptr<Engine::Audio::Memory::EffectMemory> effect;

		void load();
	}

	void init();
	void quit();
}
