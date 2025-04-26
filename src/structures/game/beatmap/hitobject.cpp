#include "structures/game/beatmap/hitobject.h" // Header
#include <fstream>
#include <ranges>
#include "logging/logger.h"
#include "logging/exceptions.h"
#include "format/file.h"
#include "utilities.hpp"

namespace Structures::Game::Beatmap::HitObjects
{
	using Format::File::Floor::Mapset::AND;

	//! TypeData
	void TypeData::read(const uint8_t& data)
	{
		const auto bitmap = std::bitset<4>(data);
		type = static_cast<Types::Game::HitObject::HitObjectType>(bitmap[0]);
		combo_colour = static_cast<uint8_t>(bitmap.to_ulong() >> 1);
	}
	void TypeData::read(const int32_t& data)
	{
		read(static_cast<uint8_t>(data));
	}
	uint8_t TypeData::to_int() const
	{
		std::bitset<4> bitmap;
		bitmap[0] = static_cast<bool>(type);
		bitmap |= std::bitset<4>(combo_colour << 1);
		return static_cast<uint8_t>(bitmap.to_ulong());
	}

	//! Floor
	void Floor::read(const std::vector<std::string>& content)
	{
		end_time = time = std::stoi(content[0]);
		type_data.read(static_cast<uint8_t>(std::stoi(content[1])));
		hit_sound = Hitsound::HitSound{ std::stoi(content[2]) };
		hit_sample = Hitsound::HitSample{ content[3] };
	}
	std::string Floor::to_string() const
	{
		std::stringstream ss;
		ss << time << AND << static_cast<int32_t>(type_data.to_int())
			<< AND << hit_sound.to_int() << AND << hit_sample.to_string();
		return ss.str();
	}

	Floor::Floor() { type_data.type = Types::Game::HitObject::HitObjectType::Floor; }

	Floor::Floor(const std::vector<std::string>& content) { Floor::read(content); }

	//! Slider
	// ::Sample
	std::string Slider::Sample::to_string() const
	{
		std::stringstream ss;
		ss << static_cast<int32_t>(normal_set) << Format::File::Floor::Mapset::HitObjects::Slider::AND << static_cast<int32_t>(addition_set);
		return ss.str();
	}

	// ::
	void Slider::read(const std::vector<std::string>& content) : Floor(content)
	{
		slides = std::stoi(content[4]);
		end_time = std::stoi(content[5]);
		for (const auto raw_edge_sound = Utilities::String::split(content[6], Format::File::Floor::Mapset::HitObjects::Slider::AND);
			const auto auto & edge_sound : raw_edge_sound)
			edge_sounds.emplace_back(std::stoi(edge_sound));
		for (const auto raw_edge_sets = Utilities::String::split(content[7], Format::File::Floor::Mapset::HitObjects::Slider::AND);
			const auto auto & edge_set : raw_edge_sets)
			edge_sets.emplace_back(edge_set);
	}
	std::string Slider::to_string() const
	{
		std::stringstream ss;
		ss << Floor::to_string() << AND << slides << AND << end_time << AND;
		for (const auto& edge_sound : edge_sounds)
			ss << edge_sound.to_int() << Format::File::Floor::Mapset::HitObjects::Slider::AND;
		ss << AND;
		for (const auto& edge_set : edge_sets)
			ss << edge_set.to_string() << Format::File::Floor::Mapset::HitObjects::Slider::AND;
		return ss.str();
	}
	Slider::Slider()
	{
		type_data.type = Types::Game::HitObject::HitObjectType::Slider;
	}

	Slider::Slider(const std::vector<std::string>& content)
	{
		read(content);
	}

	std::queue<std::weak_ptr<const Floor>> HitObjects::make_queue() const
	{
		std::queue<std::weak_ptr<const Floor>> queue;
		for (const auto& obj : data | std::views::values)
			queue.push(obj);
		return queue;
	}

	//! HitObjects
	void HitObjects::read(const std::vector<std::string>& lines)
	{
		data.clear();
		for (const auto& line : lines)
		{
			const auto contents = Utilities::String::split(line, AND);
			if (contents.size() < Floor::MINIMUM_NUM_CONTENT)
			{
				LOG_WARNING(Logging::Exceptions::FormatExceptions::HitObjects::Format_HitObjects_NotEnoughContent(line));
				continue;
			}

			const int64_t time = std::stoi(contents[0]);
			const TypeData type_data{ std::stoi(contents[1]) };
			const auto last_itr = Utilities::Container::get_last_element_iterator(data);

			switch (type_data.type)
			{
			case Types::Game::HitObject::HitObjectType::Floor:
				data.emplace_hint(last_itr, time, std::make_shared<Floor>(contents));
				break;
			case Types::Game::HitObject::HitObjectType::Slider:
				data.emplace_hint(last_itr, time, std::make_shared<Slider>(contents));
				break;
			}
		}
	}
	std::string HitObjects::to_string() const
	{
		std::stringstream ss;
		for (const auto& hit_object : data | std::views::values)
			ss << hit_object->to_string() << '\n';
		return ss.str();
	}
	std::ostream& operator<<(std::ostream& os, const HitObjects& hit_objects)
	{
		os << hit_objects.to_string();
		return os;
	}
}