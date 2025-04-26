#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>	
#include "structures/type.hpp"
#include "format/skin.h"

namespace Structures::Audio
{
	using namespace Structures::Types::Audio;

	struct MusicMemory
	{
		using CONTAINER = std::unordered_map<std::string, Music>;

		CONTAINER items;

		struct Item
		{
			MusicMemory* parent;
			CONTAINER::const_iterator item;

			void free();

			Item(MusicMemory* parent, CONTAINER::const_iterator item);
		};

		[[nodiscard]] Item get(const std::string& name);
		Item load(const std::filesystem::path& file_path, const std::string& name);
		void free(const std::string& name);
		void free_all();
	};

	struct EffectMemory
	{
		using CONTAINER = std::unordered_map<std::string, Effect>;

		CONTAINER data;

		struct Item
		{
			EffectMemory* parent = nullptr;
			CONTAINER::const_iterator item;

			void free();
			bool is_valid() const;

			Item() = default;
			Item(EffectMemory* parent, CONTAINER::const_iterator item);
		};

		[[nodiscard]] Item get(const std::string& name);
		Item load(const std::filesystem::path& file_path, const std::string& name, bool override = true);
		void load(
			const std::filesystem::path& folder_path,
			const std::filesystem::path& root_folder,
			bool recursive = true, bool name_include_extension = false, bool relative_path_name = false, bool override = true,
			const std::unordered_set<std::filesystem::path>& blacklist = {});
		void free(const std::string& name);
		void free_all();
	};
}