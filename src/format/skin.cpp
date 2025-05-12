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

			// HitObject
			data.insert(HitObject::floor);
			data.insert(HitObject::floor_overlay);
			for (const auto& val : HitObject::hit | std::views::values)
				data.insert(val);

			// HealthBar
			data.insert(HealthBar::background);
			data.insert(HealthBar::colour);

			// Score
			for (const auto& ch : Score::alphabet | std::views::values)
				data.insert(ch);

			// Pausing
			data.insert(Pausing::pause_background);
			data.insert(Pausing::fail_background);
			data.insert(Pausing::pause_continue);
			data.insert(Pausing::pause_retry);
			data.insert(Pausing::pause_back);

			// Result
			data.insert(Result::background);
			data.insert(Result::full_combo);
			for (const auto& ch : Result::alphabet | std::views::values)
				data.insert(ch);

			// Progress
			data.insert(progress);
		}
	}

	FolderNamespace::FolderNamespace()
	{
		for (const auto& val : Image::namespace_.data)
		{
			if (const auto first_slash = val.find('/');
				first_slash != std::string_view::npos)
				data.insert(val.substr(0, first_slash));
		}
	}
}
