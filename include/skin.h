#pragma once
#include <unordered_set>
#include "structures/render/texture.h"

std::unordered_set<std::string> load_skin(
	const char* skin_path,
	Structures::Render::Textures::TextureMemory& target_memory,
	const char* root_path = nullptr,
	const std::unordered_set<std::string>& only_in = {});