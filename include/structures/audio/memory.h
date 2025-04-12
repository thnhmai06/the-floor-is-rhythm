#pragma once
#include <string>
#include <unordered_map>
#include "structures/types.h"

namespace Structures::Audio::Memory
{
	using namespace Structures::Types::Audio;

	template <AudioPtrType AudioPtr>
	struct AudioMemory : std::unordered_map<std::string, AudioPtr>
	{
		void free(const std::string& name);
		void free_all();
		AudioPtr load(const char* file_path, const std::string& name);
	};
}