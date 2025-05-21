// ReSharper disable StringLiteralTypo
#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "core/type.h"
#include "engine/render/type.h"

namespace Format::Skin
{
	namespace Image
	{
		namespace Cursor
		{
			inline const std::string body = "cursor";
		}
		namespace HitObject
		{
			inline const std::string floor = "floor";
			inline const std::string floor_overlay = "floor-overlay";
			inline const std::unordered_map<Core::Type::Game::Gameplay::NoteScore, std::string> hit =
			{
				{Core::Type::Game::Gameplay::NoteScore::Miss, "hit0" },
				{Core::Type::Game::Gameplay::NoteScore::Bad, "hit50" },
				{Core::Type::Game::Gameplay::NoteScore::Good, "hit100" },
				{Core::Type::Game::Gameplay::NoteScore::Perfect, "hit300"}
			};
		}
		namespace HealthBar
		{
			inline const std::string background = "healthbar-bg";
			inline const std::string colour = "healthbar-colour";
		}
		namespace Score
		{
			using Engine::Render::Object::Alphabet;

			static inline const std::string prefix = "score-";
			inline Alphabet alphabet =
			{
				{'0', prefix + '0'},
				{'1', prefix + '1'},
				{'2', prefix + '2'},
				{'3', prefix + '3'},
				{'4', prefix + '4'},
				{'5', prefix + '5'},
				{'6', prefix + '6'},
				{'7', prefix + '7'},
				{'8', prefix + '8'},
				{'9', prefix + '9'},
				{'.', prefix + "dot"},
				{',', prefix + "comma"},
				{'x', prefix + 'x'},
				{'%', prefix + "percent"}
			};
		}
		namespace Pausing
		{
			inline const std::string fail_background = "fail-background";
			inline const std::string pause_background = "pause-background";
			inline const std::string pause_continue = "pause-continue";
			inline const std::string pause_retry = "pause-retry";
			inline const std::string pause_back = "pause-back";
		}
		namespace Result
		{
			using Engine::Render::Object::Alphabet;

			static inline const std::string prefix = "result-";
			inline Alphabet alphabet =
			{
				{'X', prefix + 'X'},
				{'S', prefix + 'S'},
				{'A', prefix + 'A'},
				{'B', prefix + 'B'},
				{'C', prefix + 'C'},
				{'D', prefix + 'D'},
			};
			const std::string background = prefix + "background";
			const std::string full_combo = prefix + "full-combo";
		}
		inline const std::string progress = "progress";

		struct Namespace
		{
			std::unordered_set<std::string> data;

			Namespace();
		};
		inline const Namespace namespace_;
	}

	namespace Sound
	{
		inline const auto fail_sound = "failsound";
		inline const auto combo_break = "combobreak";
	}

	struct FolderNamespace
	{
		std::unordered_set<std::string> data{};

		FolderNamespace();
	};
	const FolderNamespace folder_namespace;
}
