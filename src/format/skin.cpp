// ReSharper disable StringLiteralTypo
#include "format/skin.h" // Header
#include <ranges>

namespace Format::Skin
{
	namespace Image
	{
		Namespace::Namespace()
		{
			// Cursor
			data.insert(Cursor::body);
			data.insert(Cursor::trail);

			// HitObject
			data.insert(HitObject::floor);
			data.insert(HitObject::slider_focus);
			data.insert(HitObject::slider_line);
			data.insert(HitObject::slider_point);
			

			// HealthBar
			data.insert(HealthBar::background);
			data.insert(HealthBar::colour);

			// Score
			for (const auto& ch : Score::alphabet | std::views::values)
				data.insert(ch);
		}
	}

	FolderNamespace::FolderNamespace()
	{
		// Image
		for (const auto& val : Image::namespace_.data)
		{
			if (const auto first_slash = val.find('/');
				first_slash != std::string_view::npos)
				data.insert(val.substr(0, first_slash));
		}
	}
}
