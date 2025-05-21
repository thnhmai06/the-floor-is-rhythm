// ReSharper disable StringLiteralTypo
#pragma once

namespace Format::File
{
	namespace Osu
	{
		constexpr auto BEATMAP_EXTENSION = ".osz";
		constexpr auto MAPSET_EXTENSION = ".osu";
		constexpr auto STORYBOARD_EXTENSION = ".osb";
	}

	namespace Floor
	{
		namespace Mapset
		{
			namespace ConvertInformation
			{
				namespace osu
				{
					constexpr std::string_view VERSION = "//convert from Osu file format v";
					constexpr std::string_view SOURCE = "(from Osu!)";
				}
			}
			constexpr auto EXTENSION = ".tfd";
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
				constexpr std::string_view WIDESCREEN_STORYBOARD = "WidescreenStoryboard";
				constexpr std::string_view SAMPLE_SET = "SampleSet";
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
				constexpr std::string_view VC = "Velocity";
			}
			namespace HitObjects
			{
				constexpr std::string_view HEADER = "[HitObjects]";
				namespace HitSample
				{
					constexpr char AND = ':';
				}
			}
			namespace TimingPoints
			{
				constexpr std::string_view HEADER = "[TimingPoints]";
			}
			namespace Variables
			{
				constexpr std::string_view HEADER = "[Variables]";
			}
			namespace Events
			{
				constexpr std::string_view HEADER = "[Events]";
			}
		}
		namespace Config
		{
			constexpr auto EXTENSION = ".ini";
			constexpr auto FORMAT_VERSION = "; tfir config format v1";

			namespace Graphic
			{
				constexpr auto HEADER = "Graphic";

				constexpr auto FULLSCREEN = "fullscreen";
				constexpr auto WIDTH = "width";
				constexpr auto HEIGHT = "height";
			}

			namespace Audio
			{
				constexpr auto HEADER = "Audio";

				namespace Volume
				{
					constexpr auto MASTER = "master";
					constexpr auto MUSIC = "music";
					constexpr auto EFFECT = "effect";
				}
			}

			namespace Gameplay
			{
				constexpr auto HEADER = "Gameplay";
				namespace Cursor
				{
					constexpr auto CURSOR_SIZE = "cursorsize";
				}
				namespace Binding
				{
					constexpr auto L1 = "bindl1";
					constexpr auto L2 = "bindl2";
					constexpr auto R1 = "bindr1";
					constexpr auto R2 = "bindr2";
				}
			}
		}
	}

}