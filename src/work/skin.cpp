#include "work/skin.h" // Header
#include <filesystem>
#include "format/skin.h"
#include "logging/logger.h"

namespace Work::Skin
{
	std::unordered_set<std::string> load_image(
		const std::filesystem::path& folder,
		Structures::Render::Textures::TextureMemory& target_memory,
		const std::filesystem::path& root_folder,
		const std::unordered_set<std::string>& only_in)
	{
		if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)) return {}; // TODO: Exception

		std::unordered_set<std::string> changed_skin;
		try
		{
			for (const auto& entry : std::filesystem::directory_iterator(folder))
			{
				const auto& path = entry.path();
				if (const auto name = path.lexically_relative(root_folder).replace_extension().generic_string();
					entry.is_directory() && Format::Skin::folder_namespace.contains(name))
				{
					try
					{
						const auto res = load_image(path.string().c_str(), target_memory, root_folder);
						changed_skin.insert(
							std::make_move_iterator(res.begin()),
							std::make_move_iterator(res.end()));
					}
					catch (...) {} // TODO: Catch Exception in directory
				}
				else if (Format::Skin::Image::SUPPORT_EXTENSIONS.contains(path.extension().string())
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