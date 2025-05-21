#pragma once
#include <string>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <SDL3/SDL_render.h>

namespace Engine::Render::Object
{
	struct Object;
}

namespace Engine::Render::Layer
{
	struct TextureLayer;
}

namespace Engine::Render::Texture
{
	struct Memory
	{
	protected:
		using Container = std::unordered_map<std::string, SDL_Texture*>;

		Container data;
	public:
		static std::unordered_set<std::string_view> SUPPORT_EXTENSIONS;
		SDL_Renderer* renderer;

		struct Item
		{
			const Memory* memory = nullptr;
			Container::const_iterator item;

			[[nodiscard]] const std::string& get_name() const;
			[[nodiscard]] SDL_FPoint get_size() const;
			void change_target(const std::string& new_target = {}, const Memory* new_memory = nullptr);
			[[nodiscard]] bool is_valid() const;
			void render(const SDL_FRect* src_rect, const SDL_FRect* dst_rect) const;
			void clear() const;

			Item() = default;
			Item(const std::string& name, const Memory* memory);
			Item(Container::const_iterator item_in_memory, const Memory* memory);
		};

		[[nodiscard]] const Container* get_data() const;
		Item load(std::filesystem::path file_path, const std::string& name, bool override = true);
		Item load(SDL_Texture* texture, const std::string& name, bool override = true);
		std::unordered_set<std::string> load(
			const std::filesystem::path& folder_path, const std::filesystem::path& root_folder, 
			bool recursive = true, bool name_include_extension = false, bool relative_path_name = true, bool override = true,
			const std::unordered_set<std::string>& only_in = {});
		[[nodiscard]] static SDL_FPoint get_texture_size(const Container::const_iterator& texture);
		[[nodiscard]] SDL_FPoint get_texture_size(const std::string& name) const;
		[[nodiscard]] Item find(const std::string& name, bool no_log_not_found = false) const;
		Item create_new(const std::string& name, const SDL_Point& size, bool override = true, 
			const SDL_PixelFormat& format = SDL_PIXELFORMAT_RGBA8888,
			const SDL_TextureAccess& access = SDL_TEXTUREACCESS_STREAMING);
		Container::iterator rename_texture(const std::string& old_name, const std::string& new_name);
		Container::iterator move_texture(const std::string& name, Memory& to_memory);
		void free_texture(const std::string& name);
		void free_all();
		void clear();

		explicit Memory(SDL_Renderer* renderer);
		~Memory();
	};
}