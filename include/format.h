#ifndef FORMAT_H
#define FORMAT_H

#include <string_view>

namespace osu
{
	// https://osu.ppy.sh/wiki/en/Client/File_formats
	constexpr char SPACING = ':';

	namespace General
	{
		constexpr std::string_view HEADER = "[General]";
		constexpr std::string_view AUDIO_FILENAME = "AudioFilename";
		constexpr std::string_view AUDIO_LEADIN = "AudioLeadIn";
		constexpr std::string_view PREVIEW_TIME = "PreviewTime";
		constexpr std::string_view EPILEPSY_WARNING = "EpilepsyWarning";
	}

	namespace Metadata
	{
		constexpr std::string_view HEADER = "[Metadata]";
		constexpr std::string_view TITLE = "Title";
		constexpr std::string_view ARTIST = "Artist";
		constexpr std::string_view CREATOR = "Creator";
		constexpr std::string_view DIFF_NAME = "Version";
		constexpr std::string_view SOURCE = "Source";
		constexpr std::string_view TAGS = "Tags";
	}

	namespace Difficulty
	{
		constexpr std::string_view HEADER = "[Difficulty]";
		constexpr std::string_view AR = "ApproachRate";
		constexpr std::string_view OD = "OverallDifficulty";
		constexpr std::string_view HP = "HPDrainRate";
	}
}
namespace tfir
{
	namespace CONVERSION
	{
		constexpr std::string_view FORMAT_VERSION = "osu to tfir conversion v1";
	}
	constexpr std::string_view FORMAT_VERSION = "tfir file format v1";
	constexpr std::string_view SPACING = "=";
	constexpr char AND = ',';
	constexpr std::string_view ENDED = ";end";

	namespace General
	{
		constexpr std::string_view HEADER = "[General]";
		constexpr std::string_view AUDIO_FILE = "AudioFilename";
		constexpr std::string_view MUSIC_DELAY = "AudioLeadIn";
		constexpr std::string_view PREVIEW_TIMESTAMP = "PreviewTime";
		constexpr std::string_view EPILEPSY_WARNING = "EpilepsyWarning";
	}
	namespace Metadata
	{
		constexpr std::string_view HEADER = "[Metadata]";
		constexpr std::string_view TITLE = "Title";
		constexpr std::string_view ARTIST = "Artist";
		constexpr std::string_view CREATOR = "Creator";
		constexpr std::string_view DIFF_NAME = "DifficultyName";
		constexpr std::string_view SOURCE = "Source";
		constexpr std::string_view TAGS = "Tags";
	}
	namespace Difficulty
	{
		constexpr std::string_view HEADER = "[Difficulty]";
		constexpr std::string_view HP = "HPDrainRate";
		constexpr std::string_view OD = "OverallDifficulty";
		constexpr std::string_view AR = "ApproachRate";

		namespace AR_calculation
		{
			constexpr float PREEMPT_AR5 = 1200;
			constexpr float FADE_IN_AR5 = 800;
		}

		namespace OD_calculation
		{
			namespace Base
			{
				constexpr int PERFECT = 80; // 300
				constexpr int GOOD = 140; // 100
				constexpr int BAD = 200; // 50
			}

			namespace Multiply
			{
				constexpr int PERFECT = 6; // 300
				constexpr int GOOD = 8; // 100
				constexpr int BAD = 10; // 50
			}
		}
	}
}
#endif

