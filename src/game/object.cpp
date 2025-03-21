#include "game/object.h" // Header
#include <fstream>
#include "rule/file_format.h"

using namespace tfir::HitObjects;
using tfir::AND;

HitObject::Direction HitObject::get_next_direction(const Direction prev_direction, const uint8_t direction_jump)
{
	return static_cast<Direction>((static_cast<uint8_t>(prev_direction) + direction_jump) % NUM_DIRECTIONS);
}

//! Floor
void HitObject::Floor::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(direction_jump) << AND << static_cast<int32_t>(combo_jump) << AND << Type::FLOOR;
	writer << AND << hitsound.ToInt() << AND << hitsample.ToString();
	writer << '\n';
}

//! Slider
void HitObject::Slider::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(direction_jump) << AND << static_cast<int32_t>(combo_jump) << AND << Type::SLIDER << AND << end_time << AND;
	for (auto ptr = curves.begin(); ptr != curves.end(); ++ptr) {
		if (ptr != curves.begin()) writer << SLIDER_AND;
		writer << ptr->add_time << SLIDER_AND << static_cast<int32_t>(ptr->direction_jump);
	}
	writer << hitsound.ToInt() << AND << hitsample.ToString();
	writer << '\n';
}
