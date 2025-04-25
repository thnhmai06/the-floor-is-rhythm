#pragma once
#include <unordered_set>
#include <filesystem>
#include "structures/render/texture.h"
#include "structures/audio/memory.hpp"

namespace Work::Skin
{
	std::unordered_set<std::string> load_image(
		const std::filesystem::path& folder,
		Structures::Render::Textures::TextureMemory& target_memory,
		const std::filesystem::path& root_folder,
		const std::unordered_set<std::string>& only_in = {});

	/*std::unordered_set<std::string> load_sound(
		const std::filesystem::path& folder,
		Structures::Audio::Memory<Structures::Audio::Effect>& target_memory,
		const std::filesystem::path& root_folder,
		const std::unordered_set<std::string>& only_in = {});*/
}