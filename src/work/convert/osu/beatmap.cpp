#include "work/convert/osu/beatmap.h" // Header
#include <bit7z/bitarchivereader.hpp>
#include <spdlog/spdlog.h>

#include "format/file.h"

namespace Work::Convert::osu
{
	void convert_beatmap(const std::filesystem::path& path, const std::filesystem::path& output)
	{
		if (!std::filesystem::exists(path)) return;
		if (std::filesystem::is_regular_file(path) && path.extension().string() == Format::File::osu::BEATMAP_EXTENSION)
		{
			const bit7z::Bit7zLibrary lib{ "7z.dll" }; // TODO: 7zip copy here
			const bit7z::BitArchiveReader archive{ lib, path.string(), bit7z::BitFormat::Zip };
			const auto location = (output / path.filename());
			archive.extractTo(location.string());

			return convert_beatmap(location, location.parent_path());
		}

		try
		{
			if (std::filesystem::is_directory(path))
			{
				const auto dest_path = output / path.filename();
				std::filesystem::copy(path, dest_path,
						std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
				for (auto& entry : std::filesystem::recursive_directory_iterator(dest_path))
				{
					if (std::filesystem::is_regular_file(entry))
					{
						if (const auto extension = entry.path().extension().string();
							extension == Format::File::osu::MAPSET_EXTENSION)
						{
							convert_mapset(entry, entry);
							rename(entry.path(), entry.path().parent_path().string()
								+ entry.path().filename().string() + Format::File::Floor::Mapset::EXTENSION);
						}
					}
				}
			}
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			SPDLOG_ERROR("Error: ", e.what());
		}
	}
}