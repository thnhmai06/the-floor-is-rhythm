#pragma once
#include <variant>
#include "structures/render/object.h"

namespace Structures::Render::Objects
{
	// Mapset
	struct Collection
	{
	private:
		void render_in_range(const int64_t& from, const int64_t& to, const SDL_FPoint& total_offset) const;

	public:
		std::vector<std::variant<ObjectShared, PolyObjectShared>> data;
		std::vector<std::pair<int64_t, int64_t>> render_range; // Phần tử đầu tiên là 0
		SDL_FPoint offset = { 0, 0 };
		bool visible = true;
		void render(const SDL_FPoint& camera_offset) const;

		Collection() = default;
		virtual ~Collection() = default;
	};
	using CollectionShared = std::shared_ptr<Collection>;

	// Storage
	using Storage = std::list<CollectionShared>;
}