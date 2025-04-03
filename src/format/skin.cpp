#include "format/skin.h" // Header
#include <ranges>

SkinFormat::ImageStorage::ImageStorage()
{
	// Của Cursor
    insert(Cursor::BODY);
	insert(Cursor::TRAIL);
    for (const auto& val : Cursor::DIRECTION | std::views::values) {
        insert(val);
    }

	// Của HitObjects
    for (const auto& direction_skin : HitObject::HitObjectSkin) {
		for (const auto& skin : direction_skin) {
			insert(skin);
		}
    }
}

SkinFormat::FolderStorage::FolderStorage()
{
	// Của Cursor
	for (const auto& val : Cursor::DIRECTION | std::views::values) 
	{
		if (const auto first_slash = val.find('/'); 
			first_slash != std::string_view::npos) 
			insert(val.substr(0, first_slash));
	}

	// Của HitObjects
	for (const auto& val : IMAGE_STORAGE)
	{
		if (const auto first_slash = val.find('/');
			first_slash != std::string_view::npos)
			insert(val.substr(0, first_slash));
	}
}
