#pragma once

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
	namespace CONVERT
	{
		namespace osu
		{
			constexpr std::string_view VERSION = "convert from osu file format v";
			constexpr std::string_view SOURCE = "(from osu!)";
		}
	}
	constexpr std::string_view FORMAT_VERSION = "tfir file format v1";
	constexpr char SEPARATOR = '=';
	constexpr char AND = ',';

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
	}
	namespace HitObjects
	{
		constexpr std::string_view HEADER = "[HitObjects]";
		enum class Type : bool
		{
			FLOOR = 0,
			SLIDER = 1
		};
		namespace Slider
		{
			constexpr char AND = '|';
			constexpr char CURVE_AND = ':';
		}

	}
}