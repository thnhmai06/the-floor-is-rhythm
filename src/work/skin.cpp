#include "work/skin.h" // Header
#include <filesystem>
#include "format/skin.h"
#include "logging/logger.h"

namespace Work::Skin
{
	std::unordered_set<std::string> load_skin(
		const char* skin_path,
		Structures::Render::Textures::TextureMemory& target_memory,
		const char* root_path,
		const std::unordered_set<std::string>& only_in)
	{
		if (!std::filesystem::exists(skin_path) || !std::filesystem::is_directory(skin_path)) return {}; // TODO: Exception
		if (!root_path || !std::filesystem::exists(root_path)) root_path = skin_path;

		std::unordered_set<std::string> changed_skin;
		try
		{
			for (const auto& entry : std::filesystem::directory_iterator(skin_path))
			{
				const auto& path = entry.path();
				if (const auto name = path.lexically_relative(root_path).replace_extension().generic_string();
					entry.is_directory() && Format::Skin::folder_namespace.contains(name))
				{
					try
					{
						const auto res = load_skin(path.string().c_str(), target_memory, root_path);
						changed_skin.insert(
							std::make_move_iterator(res.begin()),
							std::make_move_iterator(res.end()));
					}
					catch (...) {} // TODO: Catch Exception in directory
				}
				else if (Format::Skin::SUPPORT_IMAGE_EXTENSIONS.contains(path.extension().string())
					&& Format::Skin::image_namespace.contains(name))
				{
					if (!only_in.empty() && !only_in.contains(name)) continue;
					try
					{
						target_memory.load_texture(path.string().c_str(), name);
						changed_skin.insert(name);
						// TODO: Log info
					}
					catch (...) {}
				}
			}
		}
		catch (...)
		{
			// TODO: Catch exception
		}
		return changed_skin;
	}
}