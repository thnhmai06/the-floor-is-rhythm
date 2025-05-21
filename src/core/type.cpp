#include "core/type.h" // Header

namespace Core::Type
{
	namespace Game
	{
		namespace HitSound
		{
			SampleSet string_to_sample_set(std::string str)
			{
				for (auto& c : str)
					c = std::tolower(c);
				if (str == "normal")
					return SampleSet::Normal;
				if (str == "soft")
					return SampleSet::Soft;
				if (str == "drum")
					return SampleSet::Drum;
				return SAMPLE_SET_ALL;
			}
			Addition string_to_additions(std::string str)
			{
				for (auto& c : str)
					c = std::tolower(c);
				if (str == "whistle")
					return Addition::Whistle;
				if (str == "finish")
					return Addition::Finish;
				if (str == "clap")
					return Addition::Clap;
				return ADDITION_ALL;
			}
			std::string to_string(const Addition& additions)
			{
				std::string result;
				switch (additions)
				{
				case Addition::Normal:
					result = "normal";
					break;
				case Addition::Whistle:
					result = "whistle";
					break;
				case Addition::Finish:
					result = "finish";
					break;
				case Addition::Clap:
					result = "clap";
					break;
				}
				return result;
			}
			std::string to_string(const SampleSet& sample_set)
			{
				std::string result;
				switch (sample_set)
				{
				case SampleSet::NoCustom:
					result = "";
					break;
				case SampleSet::Normal:
					result = "normal";
					break;
				case SampleSet::Soft:
					result = "soft";
					break;
				case SampleSet::Drum:
					result = "drum";
					break;
				}
				return result;
			}
		}
	}
}