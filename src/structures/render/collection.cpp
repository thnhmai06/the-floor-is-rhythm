#include "structures/render/collection.h" // Header
#include "utilities.hpp"

namespace Structures::Render::Objects
{
	void Collection::render_in_range(const int64_t& from, const int64_t& to, const SDL_FPoint& total_offset) const
	{
		const int64_t size = data.size();
		for (auto i = std::max(from, static_cast<int64_t>(0)); i <= std::min(size - 1, to); ++i)
		{
			if (const auto& object = data[i]; 
				std::holds_alternative<ObjectShared>(object))
			{
				std::get<ObjectShared>(object)->render(total_offset);
			}
			else if (std::holds_alternative<PolyObjectShared>(object))
			{
				std::get<PolyObjectShared>(object)->render(total_offset);
			}
		}
	}
	void Collection::render(const SDL_FPoint& camera_offset) const
	{
		using Utilities::Math::FPoint::operator+;

		if (!visible) return;
		const auto total_offset = camera_offset + this->offset;
		if (render_range.empty())
			render_in_range(0, data.size() - 1, total_offset);
		else for (const auto& [from, to] : render_range)
			render_in_range(from, to, total_offset);
	}
}
