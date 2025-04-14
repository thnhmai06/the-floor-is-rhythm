#pragma once
#include <variant>
#include "structures/render/layers/objects/object.h"

namespace Structures::Render::Objects
{
	// Collection
	struct Collection
	{
	private:
		void render_in_range(const size_t& from, const size_t& to, const SDL_FPoint& total_offset) const;

	public:
		std::vector<std::variant<ObjectUnique, PolyObjectUnique>> data;
		std::vector<std::pair<size_t, size_t>> render_range; // Phần tử đầu tiên là 0
		SDL_FPoint offset = { 0, 0 };
		bool visible = true;
		void render(const SDL_FPoint& camera_offset) const;

		Collection() = default;
		virtual ~Collection() = default;
	};
	using CollectionUnique = std::unique_ptr<Collection>;

	// Storage
	using Storage = std::list<CollectionUnique>;
}