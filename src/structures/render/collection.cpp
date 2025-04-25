#include "structures/render/collection.h" // Header
#include "utilities.hpp"

namespace Structures::Render::Objects
{
	void Collection::render_in_range(const size_t& from, const size_t& to, const SDL_FPoint& total_offset) const
	{
		for (auto i = std::max(from, static_cast<size_t>(0)); i <= std::min(data.size()-1, to); ++i)
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
