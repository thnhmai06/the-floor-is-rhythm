#pragma once
#include <variant>
#include "structures/render/object.h"

namespace Structures::Render::RenderObjects
{
	// Single Obj
	using RenderObjectShared = std::shared_ptr<RenderObject>;
	using RenderObjectWeak = std::weak_ptr<RenderObject>;

	using PolyRenderObjectShared = std::shared_ptr<PolyRenderObject>;
	using PolyRenderObjectWeak = std::weak_ptr<PolyRenderObject>;

	// Collection
	struct RenderObjectCollection : std::vector<std::variant<RenderObjectShared, PolyRenderObjectShared>>
	{
	protected:
		using VALUE = std::variant<RenderObjectShared, PolyRenderObjectShared>;
		using BASE = std::vector<VALUE>;

	private:
		void render_in_range(const size_t& from, const size_t& to, const SDL_FPoint& total_offset) const;

	public:
		vector<std::pair<size_t, size_t>> render_range;
		SDL_FPoint offset = { 0, 0 };
		bool visible = true;
		void render(const SDL_FPoint& camera_offset);

		RenderObjectCollection() = default;
		~RenderObjectCollection();
	};

	// Storage
	using RenderObjectStorage = std::list<RenderObjectCollection>;
}