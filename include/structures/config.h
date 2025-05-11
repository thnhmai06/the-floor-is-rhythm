#pragma once
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_scancode.h>
#include <filesystem>
#include <INIReader.h>

namespace fs = std::filesystem;

namespace Structures::Config
{
	struct Config
	{
		[[nodiscard]] virtual std::string to_string() const = 0;

		virtual ~Config() = default;
		friend std::ostream& operator<<(std::ostream& os, const Config& config);
	};
	struct INISection : Config
	{
		virtual void read(const INIReader& reader) = 0;
	};
	struct Section : Config
	{
		virtual void read(const std::vector<std::string>& contents) = 0;
	};

	struct UserConfig : Config
	{
		fs::path file_path;

		struct Graphic : INISection
		{
			bool fullscreen = false;
			SDL_Point window_size = {.x = 1366, .y = 768 };

			void read(const INIReader& reader) override;
			[[nodiscard]] std::string to_string() const override;

			Graphic() = default;
			explicit Graphic(const INIReader& reader);
		} graphic;
		struct Audio : INISection
		{
			struct Volume : INISection
			{
				float master = 1.0f;
				float music = 0.5f;
				float effect = 0.5f;

				void read(const INIReader& reader) override;
				[[nodiscard]] std::string to_string() const override;
			} volume;

			void read(const INIReader& reader) override;
			[[nodiscard]] std::string to_string() const override;

			Audio() = default;
			explicit Audio(const INIReader& reader);
		} audio;
		struct Gameplay : INISection
		{
			struct Cursor : INISection
			{
			protected:
				const Graphic* graphic;
				static constexpr float BASE_SIZE = 0.125f;// % so với min (window_height, window_width)

			public:
				float size_percent = 0.6f;

				[[nodiscard]] float get_pixel_size() const;
				void read(const INIReader& reader) override;
				[[nodiscard]] std::string to_string() const override;

				explicit Cursor(const Graphic* graphic);
			} cursor;
			struct Binding : INISection
			{
				SDL_Scancode l1 = SDL_SCANCODE_D;
				SDL_Scancode l2 = SDL_SCANCODE_F;
				SDL_Scancode r1 = SDL_SCANCODE_J;
				SDL_Scancode r2 = SDL_SCANCODE_K;

				[[nodiscard]] bool is_click_key(const SDL_Scancode& key) const;
				[[nodiscard]] bool is_left_key(const SDL_Scancode& key) const;
				[[nodiscard]] bool is_right_key(const SDL_Scancode& key) const;
				void read(const INIReader& reader) override;
				[[nodiscard]] std::string to_string() const override;
			} binding;

			void read(const INIReader& reader) override;
			[[nodiscard]] std::string to_string() const override;

			friend std::ostream& operator<<(std::ostream& os, const Gameplay& gameplay);
			explicit Gameplay(const Graphic* graphic);
			explicit Gameplay(const Graphic* graphic, const INIReader& reader);
		} gameplay;

		void read(const fs::path& file_path);
		[[nodiscard]] std::string to_string() const override;
		void write(const fs::path& file_path) const;

		UserConfig();
		explicit UserConfig(const fs::path& file_path);
	};
}
