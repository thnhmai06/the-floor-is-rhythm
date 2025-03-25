#include "game/object.h" // Header
#include <fstream>
#include "rule/file_format.h"

using namespace tfir::HitObjects;
using tfir::AND;

Direction HitObject::get_next_direction(const Direction prev_direction, const uint8_t direction_jump)
{
	return static_cast<Direction>((static_cast<uint8_t>(prev_direction) + direction_jump) % NUM_DIRECTIONS);
}

//! Floor
void HitObject::Floor::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(direction_jump) << AND << static_cast<int32_t>(combo_jump) << AND <<
		static_cast<bool>(Type::FLOOR);
	writer << AND << hitsound.ToInt() << AND << hitsample.ToString();
	writer << '\n';
}

//! Slider
void HitObject::Slider::write(std::ofstream& writer) const
{
	writer << time << AND << static_cast<int32_t>(direction_jump) << AND << static_cast<int32_t>(combo_jump) << AND <<
		static_cast<bool>(Type::SLIDER) << AND << end_time << AND;
	for (auto ptr = curves.begin(); ptr != curves.end(); ++ptr) {
		if (ptr != curves.begin()) writer << tfir::HitObjects::Slider::AND;
		writer << ptr->add_time << tfir::HitObjects::Slider::CURVE_AND << static_cast<int32_t>(ptr->direction_jump);
	}
	writer << AND;
	writer << hitsound.ToInt() << AND << hitsample.ToString();
	writer << '\n';
}

//! HitObjects
void HitObjects::write(std::ofstream& writer) const
{
	writer << HEADER << '\n';
	for (auto ptr = begin(); ptr != end(); ++ptr) 
	{
		std::visit(
			[&writer](const auto& object) { object.write(writer); }, 
			ptr->second
		);
	}
	writer << '\n';
}
