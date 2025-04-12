#pragma once
#include <variant>
#include "structures/render/layers/objects/object.h"

namespace Structures::Render::Objects
{
	// Single Obj
	using ObjectShared = std::shared_ptr<Object>;
	using ObjectWeak = std::weak_ptr<Object>;

	using PolyObjectShared = std::shared_ptr<PolyObject>;
	using PolyObjectWeak = std::weak_ptr<PolyObject>;

	// Collection
	struct Collection : std::vector<std::variant<ObjectShared, PolyObjectShared>>
	{
	protected:
		using VALUE = std::variant<ObjectShared, PolyObjectShared>;
		using BASE = std::vector<VALUE>;

	private:
		void render_in_range(const size_t& from, const size_t& to, const SDL_FPoint& total_offset) const;

	public:
		vector<std::pair<size_t, size_t>> render_range; // Phần tử đầu tiên là 0
		SDL_FPoint offset = { 0, 0 };
		bool visible = true;
		void render(const SDL_FPoint& camera_offset) const;

		Collection() = default;
		~Collection();
	};
	using CollectionUnique = std::unique_ptr<Collection>;

	// Storage
	using Storage = std::list<CollectionUnique>;
}