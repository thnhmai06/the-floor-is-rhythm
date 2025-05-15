#include "core/work/convert/osu/beatmap.h" // Header
#include <algorithm>
#include <bit7z/bitarchivereader.hpp>
#include <spdlog/spdlog.h>
#include <filesystem>
#include "format/file.h"
#include "core/work/convert/osu/mapset.h"
#include "logging/exceptions.h"
#include "logging/logger.h"

namespace fs = std::filesystem;

namespace Core::Work::Convert::Osu
{
	void convert_beatmap(fs::path path, const fs::path& output)
	{
		path = Utilities::Path::normalize_path(path);

		if (!fs::exists(path)) return;
		if (fs::is_regular_file(path) && path.extension().string() == Format::File::Osu::BEATMAP_EXTENSION)
		{
			bit7z::Bit7zLibrary lib{ "7z.dll" }; //TODO: Exception No lib here
			const bit7z::BitArchiveReader archive{ lib, path.string(), bit7z::BitFormat::Zip };

			std::string folder_name = path.stem().string();
			std::erase(folder_name, '.');
			const auto location = output / folder_name;
			archive.extractTo(location.string());

			return convert_beatmap(location, location.parent_path());
		}
		if (fs::is_directory(path))
		{
			const auto valid_name = path.filename().string();;

			auto dest_path = output / path.filename();
			if (path != dest_path)
			{
				try
				{
					fs::copy(path, dest_path, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
				}
				catch (...)
				{
					LOG_ERROR(Logging::Exceptions::FileExceptions::File_Copy_Failed(path));
					return;
				}
			}
			for (auto&& entry : fs::recursive_directory_iterator(dest_path))
			{
				auto entry_path = Utilities::Path::normalize_path(entry.path());

				if (fs::is_regular_file(entry_path))
				{
					if (const auto extension = entry_path.extension();
						extension == Format::File::Osu::MAPSET_EXTENSION)
					{
						convert_mapset(entry_path, entry_path);

						auto new_path = entry_path;
						const auto mapset_extension = fs::path{ Format::File::Floor::Mapset::EXTENSION };
						new_path.replace_extension(mapset_extension);

						fs::rename(entry_path, new_path);
					}
				}
			}
		}
	}
}