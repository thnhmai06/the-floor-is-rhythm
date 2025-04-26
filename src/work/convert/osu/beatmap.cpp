#include "work/convert/osu/beatmap.h" // Header
#include <bit7z/bitarchivereader.hpp>
#include <spdlog/spdlog.h>
#include <filesystem>
#include "format/file.h"

namespace fs = std::filesystem;

namespace Work::Convert::osu
{
	void convert_beatmap(const fs::path& path, const fs::path& output)
	{
		if (!fs::exists(path)) return;
		if (fs::is_regular_file(path) && path.extension().string() == Format::File::Osu::BEATMAP_EXTENSION)
		{
			bit7z::Bit7zLibrary lib{ "7z.dll" }; //TODO: Exception No lib here

			const bit7z::BitArchiveReader archive{ lib, path.string(), bit7z::BitFormat::Zip };
			const auto location = (output / path.stem());
			archive.extractTo(location.string());

			return convert_beatmap(location, location.parent_path());;
		}

		if (fs::is_directory(path))
		{
			auto dest_path = output / path.filename();
			if (path != dest_path)
				fs::copy(path, dest_path, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
			for (auto&& entry : fs::recursive_directory_iterator(dest_path))
			{
				if (fs::is_regular_file(entry))
				{
					if (const auto extension = entry.path().extension().string();
						extension == Format::File::Osu::MAPSET_EXTENSION)
					{
						convert_mapset(entry, entry);

						auto new_path = entry.path();
						const auto mapset_extension = fs::path{ Format::File::Floor::Mapset::EXTENSION };
						new_path.replace_extension(mapset_extension);

						fs::rename(entry.path(), new_path);
					}
				}
			}
		}
	}
}