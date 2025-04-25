#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>	
#include "structures/types.h"
#include "format/skin.h"

namespace Structures::Audio
{
	using namespace Structures::Types::Audio;

	template <AudioPtrType AudioPtr>
	struct Memory
	{
	protected:
		using CONTAINER = std::unordered_map<std::string, AudioPtr>;

	public:
		CONTAINER items;

		struct Item
		{
			Memory* parent;
			typename CONTAINER::iterator item;

			void free()
			{
				if (item && parent)
				{
					parent->free(*item->first);
					item = nullptr;
				}
			}
		};

		[[nodiscard]] Item get(const std::string& name) const
		{
			if (const auto it = items.find(name); it != items.end())
				return { this, &it->first, &it->second };
			return { nullptr, nullptr, nullptr };
		}
		Item load(const std::filesystem::path& file_path, const std::string& name);
		void load(const std::filesystem::path& folder_path, const bool recursive = true)
		{
			if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path))
				for (const auto& entry : std::filesystem::recursive_directory_iterator(folder_path))
				{
					if (recursive && std::filesystem::is_directory(entry)) load(entry);
					else if (std::filesystem::is_regular_file(entry))
					{
						const auto extension = entry.path().extension().string();
						if (!Format::Skin::Sound::SUPPORT_EXTENSIONS.contains(extension)) continue;
						const auto name = entry.path().filename().string();
						load(entry, name);
					}
				}
		}
		void free(const std::string& name);
		void free_all();
	};
}