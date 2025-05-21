#pragma once
#include "structures/game/mapset.h"
#include "engine/render/object.h"
#include "engine/render/object/number.h"

namespace Structures::Screen::Gameplay::Score
{
	namespace Render
	{
		using namespace Engine::Render::Object;

		struct Score : Collection
		{
			std::shared_ptr<HorizontalNumber<float>> score;
			std::shared_ptr<HorizontalNumber<float>> accuracy;
			std::shared_ptr<HorizontalNumber<unsigned long>> combo;

			Score(const Memory& skin, const float* score, const float* accuracy, const unsigned long* combo);
		};
	}
	namespace Logic
	{
		struct Score
		{
		protected:
			const Game::Beatmap::Mapset* mapset;
			float score = 0;
			const float* score_multiplier;

		public:
			class Combo
			{
			protected:
				unsigned long max_combo = 0;
				unsigned long current_combo = 0;

			private:
				void add_combo(const uint32_t& num = 1);
				void reset(bool reset_max_combo = false);

				friend Score;
			public:
				[[nodiscard]] const unsigned long* get_current_combo() const;
				[[nodiscard]] const unsigned long* get_max_combo() const;

			} combo;
			class Accuracy
			{
			protected:
				float accuracy = 1;
				unsigned long count_perfect = 0;
				unsigned long count_good = 0;
				unsigned long count_bad = 0;
				unsigned long count_miss = 0;

			private:
				void update(const Core::Type::Game::Gameplay::NoteScore& score, const unsigned long& num = 1);
				void reset();

				friend Score;

			public:
				[[nodiscard]] const float* get_accuracy() const { return &accuracy; }
				[[nodiscard]] const unsigned long* get_count_300() const { return &count_perfect; }
				[[nodiscard]] const unsigned long* get_count_100() const { return &count_good; }
				[[nodiscard]] const unsigned long* get_count_50() const { return &count_bad; }
				[[nodiscard]] const unsigned long* get_count_miss() const { return &count_miss; }
				[[nodiscard]] char get_grade() const;
				[[nodiscard]] unsigned long get_elapsed_objects_num() const;
			} accuracy;

			void update(const Core::Type::Game::Gameplay::NoteScore& note_score = Core::Type::Game::Gameplay::NoteScore::Skip, const uint32_t& num = 1);
			[[nodiscard]] const float* get_score() const;
			[[nodiscard]] const float* get_score_multiplier() const;
			
			Core::Type::Game::Gameplay::NoteScore get_floor_score(
				const Game::Beatmap::HitObjects::Floor& floor,
				uint16_t& click_num,
				const int64_t& click_time) const;
			void reset();

			explicit Score(const Game::Beatmap::Mapset& mapset, const float* score_multiplier);
		};
	}
}