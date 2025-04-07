#pragma once
#include <chrono>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <SDL3_mixer/SDL_mixer.h>
#include "template.h"

namespace Utilities
{
	namespace Math
	{
		inline bool is_bit_enabled(const std::int32_t& value, const std::int32_t& bitmask) { return (value & bitmask) != 0; }
		inline bool is_equal_float(const float& variable, const float& value, const float& epsilon = 0.01f) { return abs(variable - value) <= epsilon; }
		inline float centre(const float& size, const float& window_size) { return (window_size - size) / 2; }
		inline float get_rotation_angle(const Template::Game::Direction::Direction& rotation, const bool clockwise = true)
		{
			switch (rotation)
			{
			case Template::Game::Direction::Direction::RIGHT:
				return 0.0f;
			case Template::Game::Direction::Direction::LEFT:
				return 180.0f;
			case Template::Game::Direction::Direction::UP:
				return clockwise ? -90.0f : 90.0f;
			case Template::Game::Direction::Direction::DOWN:
				return clockwise ? 90.0f : -90.0f;
			}
			return 0.0f;
		}
		inline float max_float(const float& a, const float& b) { return a > b ? a : b; }

		namespace FPoint
		{
			inline SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b) { return SDL_FPoint{ a.x + b.x, a.y + b.y }; }
			inline SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b) { return SDL_FPoint{ a.x - b.x, a.y - b.y }; }
			inline SDL_FPoint& operator+=(SDL_FPoint& a, const SDL_FPoint& b) { a.x += b.x; a.y += b.y; return a; }
			inline SDL_FPoint& operator-=(SDL_FPoint& a, const SDL_FPoint& b) { a.x -= b.x; a.y -= b.y; return a; }
			inline SDL_FPoint operator-(const SDL_FPoint& a) { return SDL_FPoint{ -a.x, -a.y }; }
		}
	}
	namespace Audio
	{
		inline int32_t get_volume(const int32_t v)
		{
			if (v < 0) return -1;
			return (MIX_MAX_VOLUME * v) / 100;
		}
		inline int32_t get_real_volume(const int32_t v)
		{
			return (v * 100) / MIX_MAX_VOLUME;
		}
	}
	namespace Time
	{
		inline std::string get_current_time(const char* format = "%Y-%m-%d_%H-%M-%S")
		{
			const auto now = std::chrono::system_clock::now();
			const auto time_t_now = std::chrono::system_clock::to_time_t(now);

			std::stringstream ss;
			ss << std::put_time(std::localtime(&time_t_now), format);
			return ss.str();
		}
	}
	namespace String
	{
		inline std::string trim(std::string str)
		{
			unsigned long long start = 0;
			for (const auto& c : str)
			{
				if (c == ' ' || c == '\t' || c == '\n' || c == '\r') start++;
				else break;
			}
			if (str.empty()) return str;
			while (str.back() == ' ' || str.back() == '\t' || str.back() == '\n' || str.back() == '\r') str.pop_back();
			return str.substr(start);
		}
		inline std::vector<std::string> split(const std::string& input, char delimiter = ',', bool only_two_part = false)
		{
			std::vector<std::string> result;
			std::stringstream stream(input);
			std::string current_line;
			if (only_two_part)
			{
				std::getline(stream, current_line, delimiter);
				result.push_back(trim(current_line));
				std::getline(stream, current_line);
				result.push_back(trim(current_line));
			}
			else
			{
				while (std::getline(stream, current_line, delimiter))
					result.push_back(trim(current_line));
			}
			return result;
		}
	}
	namespace Render
	{
		inline SDL_FRect merge_pos_size(const SDL_FPoint& pos, const SDL_FPoint& size) { return SDL_FRect{ pos.x, pos.y, size.x, size.y }; }
		inline SDL_FPoint get_pos_from_rect(const SDL_FRect& rect) { return SDL_FPoint{ rect.x, rect.y }; }
		inline SDL_FPoint get_size_from_rect(const SDL_FRect& rect) { return SDL_FPoint{ rect.w, rect.h }; }
	}
}
