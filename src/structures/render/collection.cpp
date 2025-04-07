#include "structures/render/collection.h" // Header
#include "utilities.h"

// Structures::Render::RenderObjects::RenderObjectCollection
using Structures::Render::RenderObjects::RenderObjectCollection;
void RenderObjectCollection::render_in_range(const size_t& from, const size_t& to, const SDL_FPoint& total_offset) const
{
	for (auto i = from; i < to; ++i)
		std::visit([&](auto& object)
			{
				if (!object) return;
				object->render(total_offset);
			}, (*this)[i]);
}
void RenderObjectCollection::render(const SDL_FPoint& camera_offset)
{
	using Utilities::Math::FPoint::operator+;

	if (!visible) return;
	const auto total_offset = camera_offset + this->offset;
	if (render_range.empty())
		render_in_range(0, size() - 1, total_offset);
	else for (const auto& [from, to] : render_range)
		render_in_range(from, to, total_offset);
}
RenderObjectCollection::~RenderObjectCollection()
{
	for (auto& object : *this)
	{
		std::visit([](auto& object)
			{
				if (object)
					object.reset();
			}, object);
	}
	render_range.clear();
}
