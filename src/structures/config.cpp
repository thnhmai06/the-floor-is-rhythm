#include "structures/config.h" // Header
#include <algorithm>
#include <cmath>
#include <fstream>
#include "logging/exceptions.h"
#include "logging/logger.h"
#include "format/file.h"

namespace Structures::Config
{
	//! Config (BASE)
	std::ostream& operator<<(std::ostream& os, const Config& config)
	{
		return os << config.to_string();
	}

	//! Graphic
	void UserConfig::Graphic::read(const INIReader& reader)
	{
		using namespace Format::File::Floor::Config::Graphic;

		fullscreen = reader.GetBoolean(HEADER, FULLSCREEN, fullscreen);
		window_size.x = reader.GetInteger(HEADER, WIDTH, window_size.x);
		window_size.y = reader.GetInteger(HEADER, HEIGHT, window_size.y);
	}
	std::string UserConfig::Graphic::to_string() const
	{
		using namespace Format::File::Floor::Config::Graphic;

		std::stringstream ss;
		ss << '[' << HEADER << ']' << '\n';
		ss << FULLSCREEN << '=' << fullscreen << '\n';
		ss << WIDTH << '=' << window_size.x << '\n';
		ss << HEIGHT << '=' << window_size.y;
		return ss.str();
	}
	UserConfig::Graphic::Graphic(const INIReader& reader) { Graphic::read(reader); }

	//! Audio
	// ::Volume
	void UserConfig::Audio::Volume::read(const INIReader& reader)
	{
		using namespace Format::File::Floor::Config::Audio;

		master = static_cast<float>(reader.GetReal(HEADER, Format::File::Floor::Config::Audio::Volume::MASTER, master));
		music = static_cast<float>(reader.GetReal(HEADER, Format::File::Floor::Config::Audio::Volume::MUSIC, music));
		effect = static_cast<float>(reader.GetReal(HEADER, Format::File::Floor::Config::Audio::Volume::EFFECT, effect));
	}
	std::string UserConfig::Audio::Volume::to_string() const
	{
		using namespace Format::File::Floor::Config::Audio::Volume;

		std::stringstream ss;
		ss << MASTER << '=' << master << '\n';
		ss << MUSIC << '=' << music << '\n';
		ss << EFFECT << '=' << effect;
		return ss.str();
	}
	// ::
	void UserConfig::Audio::read(const INIReader& reader)
	{
		volume.read(reader);
	}
	std::string UserConfig::Audio::to_string() const
	{
		using namespace Format::File::Floor::Config::Audio;

		std::stringstream ss;
		ss << '[' << HEADER << ']' << '\n';
		ss << volume.to_string();
		return ss.str();
	}
	UserConfig::Audio::Audio(const INIReader& reader) { Audio::read(reader); }

	//! Gameplay
	// ::Cursor
	float UserConfig::Gameplay::Cursor::get_pixel_size() const
	{
		const auto full_size = std::min(graphic->window_size.x, graphic->window_size.y) * BASE_SIZE;
		return size_percent * full_size;
	}
	void UserConfig::Gameplay::Cursor::read(const INIReader& reader)
	{
		using namespace Format::File::Floor::Config::Gameplay;
		size_percent = reader.GetReal(HEADER, Format::File::Floor::Config::Gameplay::Cursor::CURSOR_SIZE, size_percent);
	}
	std::string UserConfig::Gameplay::Cursor::to_string() const
	{
		using namespace Format::File::Floor::Config::Gameplay::Cursor;

		std::stringstream ss;
		ss << CURSOR_SIZE << '=' << size_percent;
		return ss.str();
	}
	UserConfig::Gameplay::Cursor::Cursor(const Graphic* graphic) : graphic(graphic)
	{
	}
	// ::Binding
	bool UserConfig::Gameplay::Binding::is_click_key(const SDL_Scancode& key) const
	{
		return key == l1 || key == l2 || key == r1 || key == r2;
	}
	bool UserConfig::Gameplay::Binding::is_left_key(const SDL_Scancode& key) const
	{
		return key == l1 || key == l2;
	}
	bool UserConfig::Gameplay::Binding::is_right_key(const SDL_Scancode& key) const
	{
		return key == r1 || key == r2;
	}
	void UserConfig::Gameplay::Binding::read(const INIReader& reader)
	{
		using namespace Format::File::Floor::Config::Gameplay;
		l1 = static_cast<SDL_Scancode>(reader.GetInteger(HEADER, Format::File::Floor::Config::Gameplay::Binding::L1, l1));
		l2 = static_cast<SDL_Scancode>(reader.GetInteger(HEADER, Format::File::Floor::Config::Gameplay::Binding::L2, l2));
		r1 = static_cast<SDL_Scancode>(reader.GetInteger(HEADER, Format::File::Floor::Config::Gameplay::Binding::R1, r1));
		r2 = static_cast<SDL_Scancode>(reader.GetInteger(HEADER, Format::File::Floor::Config::Gameplay::Binding::R2, r2));
	}
	std::string UserConfig::Gameplay::Binding::to_string() const
	{
		using namespace Format::File::Floor::Config::Gameplay::Binding;
		std::stringstream ss;
		ss << L1 << '=' << l1 << '\n';
		ss << L2 << '=' << l2 << '\n';
		ss << R1 << '=' << r1 << '\n';
		ss << R2 << '=' << r2;
		return ss.str();
	}
	// ::
	void UserConfig::Gameplay::read(const INIReader& reader)
	{
		cursor.read(reader);
		binding.read(reader);
	}
	std::string UserConfig::Gameplay::to_string() const
	{
		using namespace Format::File::Floor::Config::Gameplay;
		std::stringstream ss;
		ss << '[' << HEADER << ']' << '\n';
		ss << cursor.to_string() << '\n';
		ss << binding.to_string();
		return ss.str();
	}
	std::ostream& operator<<(std::ostream& os, const UserConfig::Gameplay& gameplay)
	{
		os << gameplay.to_string();
		return os;
	}
	UserConfig::Gameplay::Gameplay(const Graphic* graphic) : cursor(graphic)
	{
	}
	UserConfig::Gameplay::Gameplay(const Graphic* graphic, const INIReader& reader) : Gameplay(graphic) { Gameplay::read(reader); }

	//! TOTAL
	void UserConfig::read(const fs::path& file_path)
	{
		const INIReader reader(file_path.string());

		if (const auto line = reader.ParseError() != 0)
		{
			LOG_ERROR(Logging::Exceptions::File::File_Config_Failed(file_path, line));
			return;
		}

		graphic.read(reader);
		audio.read(reader);
		gameplay.read(reader);
	}
	std::string UserConfig::to_string() const
	{
		using namespace Format::File::Floor::Config;

		std::stringstream ss;

		ss << FORMAT_VERSION << "\n\n";
		ss << graphic << "\n\n";
		ss << audio << "\n\n";
		ss << gameplay;

		return ss.str();
	}
	void UserConfig::write(const fs::path& file_path) const
	{
		std::ofstream writer(file_path);

		if (!writer)
		{
			LOG_ERROR(Logging::Exceptions::File::File_Open_Failed(file_path));
			return;
		}

		writer << to_string();
		writer.close();
	}
	UserConfig::UserConfig() : gameplay(&graphic) {}
	UserConfig::UserConfig(const fs::path& file_path) : UserConfig() { read(file_path); }

}
