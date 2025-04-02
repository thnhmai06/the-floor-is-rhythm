#include "skin.h" // Header
#include <filesystem>
#include "rule/skin_format.h"
#include "logger/logging.h"

void load_skin(const char* skin_path, TextureMemory& target_memory, const char* root_path)
{
	if (!std::filesystem::exists(skin_path) || !std::filesystem::is_directory(skin_path)) return; // TODO: Exception
	if (!root_path || !std::filesystem::exists(root_path)) root_path = skin_path;
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(skin_path))
		{
			const auto path = entry.path();
			const auto skin_name = path.lexically_relative(root_path).string();
			if (entry.is_directory() && SkinFormat::FOLDER_STORAGE.contains(path.string()))
			{
				try
				{
					load_skin(path.string().c_str(), target_memory, root_path);
				} catch (...) {} // TODO: Catch Exception in directory
			}
			else if (SkinFormat::SUPPORT_IMAGE_EXTENSIONS.contains(path.extension().string()) 
				&& SkinFormat::IMAGE_STORAGE.contains(skin_name))
			{
				try
				{
					target_memory.load_texture(path.string().c_str(), skin_name);
					// TODO: Log info
				} catch (...) {}
			}
		}
	} catch (...)
	{
		// TODO: Catch exception
	}
}
