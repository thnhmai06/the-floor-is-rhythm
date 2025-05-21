#pragma once
#include <concepts>
#include <SDL3_mixer/SDL_mixer.h>

namespace Engine::Audio
{
	using Music = Mix_Music*;
	using Effect = Mix_Chunk*;

	template <typename T>
	concept AudioTypename = std::same_as<T, Music> || std::same_as<T, Effect>;
}
