#pragma once

namespace tfir_file
{
	namespace Beatmap
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
			constexpr std::string_view VELOCITY = "Velocity";
		}
		namespace HitObjects
		{
			constexpr std::string_view HEADER = "[HitObjects]";
			namespace Slider
			{
				constexpr char AND = '|';
				constexpr char CURVE_AND = ':';
			}
			namespace HitSample
			{
				constexpr char DELIMETER = ':';
			}
		}
		namespace TimingPoints
		{
			constexpr std::string_view HEADER = "[TimingPoints]";
		}
	}
}