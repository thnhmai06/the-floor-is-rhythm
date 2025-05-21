#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>	
#include <unordered_set>
#include "engine/audio/type.h"

namespace Engine::Audio::Memory
{
	struct MusicMemory
	{
		using Container = std::unordered_map<std::string, Music>;

		Container items;

		struct Item
		{
			const MusicMemory* parent = nullptr;
			Container::const_iterator item;

			void reset();
			[[nodiscard]] bool is_valid() const;

			Item() = default;
			Item(const MusicMemory* parent, Container::const_iterator item);
		};

		[[nodiscard]] Item find(const std::string& name) const;
		Item load(const std::filesystem::path& file_path, const std::string& name);
		void free(const std::string& name);
		void free(const Item& item);
		void free_all();

		~MusicMemory();
	};

	struct EffectMemory
	{
		using Container = std::unordered_map<std::string, Effect>;
		static std::unordered_set<std::string_view> SUPPORT_EXTENSIONS;

		Container data;

		struct Item
		{
			const EffectMemory* parent = nullptr;
			Container::const_iterator item;

			bool is_valid() const;

			Item() = default;
			Item(const EffectMemory* parent, Container::const_iterator item);
		};

		[[nodiscard]] Item find(const std::string& name, bool no_log_not_found = false) const;
		Item load(const std::filesystem::path& file_path, const std::string& name, bool override = true);
		void load(
			const std::filesystem::path& folder_path,
			const std::filesystem::path& root_folder,
			bool recursive = true, bool name_include_extension = false, bool relative_path_name = false, bool override = true,
			const std::unordered_set<std::filesystem::path>& blacklist = {});
		void free(const std::string& name);
		void free(const Item& item);
		void free_all();

		~EffectMemory();
	};
}