#pragma once
#include <string>
#include <unordered_map>
#include "structures/types.h"

namespace Structures::Audio
{
	using namespace Structures::Types::Audio;

	template <AudioPtrType AudioPtr>
	struct Memory
	{
	protected:
		using CONTAINER = std::unordered_map<std::string, AudioPtr>;

	public:
		CONTAINER items;

		struct Item
		{
			Memory* parent;
			typename CONTAINER::iterator item;

			void free();
		};
		
		Item load(const char* file_path, const std::string& name);
		[[nodiscard]] Item get(const std::string& name) const;
		void free(const std::string& name);
		void free_all();
	};
}