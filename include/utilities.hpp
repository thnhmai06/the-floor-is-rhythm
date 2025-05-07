#pragma once
#include <chrono>
#include <cmath>
#include <iomanip>
#include <map>
#include <numbers>
#include <queue>
#include "structures/type.hpp"

namespace Utilities
{
	namespace Math
	{
		inline bool is_equal(const float& variable, const float& value, const float& epsilon = 0.001f) { return abs(variable - value) <= epsilon; }
		inline bool is_integer(const float& value, const float& epsilon = 0.001f) { return is_equal(std::floor(value), value, epsilon); }
		inline float centre(const float& size, const float& window_size) { return (window_size - size) / 2; }
		inline float degree_to_radian(const float& degree) { return degree * std::numbers::pi_v<float> / 180.0f; }
		inline float radian_to_degree(const float& radian) { return radian * 180.0f / std::numbers::pi_v<float>; }
		inline double round(const double& value, const uint8_t& precision_num = 0)
		{
			const double factor = std::pow(10.0, precision_num);
			return std::round(value * factor) / factor;
		}
		inline std::pair<double, double> decimal_separate(const double& value)
		{
			double integer;
			const double fraction = std::modf(value, &integer);
			return { integer, fraction };
		}
		inline size_t count_digits(const size_t& value)
		{
			if (value < 1) return 1;
			return static_cast<size_t>(std::log10(value)) + 1;
		}
		template <typename IntegerType>
		IntegerType circle_mod(const IntegerType& value, const IntegerType& mod)
		{
			if (mod == 0) return value;
			return (value % mod + mod) % mod;
		}

		namespace FPoint
		{
			inline SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b) { return SDL_FPoint{ a.x + b.x, a.y + b.y }; }
			inline SDL_FPoint& operator+=(SDL_FPoint& a, const SDL_FPoint& b) { a.x += b.x; a.y += b.y; return a; }

			inline SDL_FPoint operator-(const SDL_FPoint& a) { return SDL_FPoint{ -a.x, -a.y }; }
			inline SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b) { return a + (-b); }
			inline SDL_FPoint& operator-=(SDL_FPoint& a, const SDL_FPoint& b) { return a += (-b); }

			inline SDL_FPoint operator*(const SDL_FPoint& a, const SDL_FPoint& b) { return SDL_FPoint{ a.x * b.x, a.y * b.y }; }
			inline SDL_FPoint operator*(const SDL_FPoint& point, const float& value) { return SDL_FPoint{ point.x * value, point.y * value }; }
			inline SDL_FPoint& operator*=(SDL_FPoint& a, const SDL_FPoint& b) { a.x *= b.x; a.y *= b.y; return a; }
			inline SDL_FPoint& operator*=(SDL_FPoint& point, const float& value) { point.x *= value; point.y *= value; return point; }

			inline SDL_FPoint operator/(const SDL_FPoint& a, const SDL_FPoint& b) { return SDL_FPoint{ a.x / b.x, a.y / b.y }; }
			inline SDL_FPoint operator/(const SDL_FPoint& point, const float& value) { return SDL_FPoint{ point.x / value, point.y / value }; }
			inline SDL_FPoint& operator/=(SDL_FPoint& a, const SDL_FPoint& b) { a.x /= b.x; a.y /= b.y; return a; }
			inline SDL_FPoint& operator/=(SDL_FPoint& point, const float& value) { point.x /= value; point.y /= value; return point; }

			inline bool operator==(const SDL_FPoint& a, const SDL_FPoint& b) { return  is_equal(a.x, b.x) && is_equal(a.y, b.y); }
			inline bool operator!=(const SDL_FPoint& a, const SDL_FPoint& b) { return !operator==(a, b); }
		}

		template <typename NumberType>
		//! HÀM NÀY KHÔNG PHÙ HỢP CHO CẶP 2 SỐ
		double to_percent(const NumberType& value, const NumberType& from, const NumberType& to)
		{
			if (from == to) return 1.0f;
			return static_cast<double>(value - from) / static_cast<double>(to - from);
		}
		template <typename NumberType>
		//! HÀM NÀY KHÔNG PHÙ HỢP CHO CẶP 2 SỐ
		NumberType to_value(const double& percent, const NumberType& from, const NumberType& to)
		{
			if (from == to) return to;
			return static_cast<NumberType>(from + (to - from) * percent);
		}

		template <typename ComparableType>
		bool in_range(const ComparableType& min, const ComparableType& max,
			const ComparableType& value, const bool include_equal_left = true, const bool include_equal_right = true)
		{
			if (include_equal_left && include_equal_right) return (min <= value && value <= max);
			if (include_equal_left) return (min <= value && value < max);
			if (include_equal_right) return (min < value && value <= max);
			return (min < value && value < max);
		}
		template <typename ComparableType>
		bool in_offset_range(const ComparableType& correct_value, const ComparableType& offset,
			const ComparableType& value, const bool include_equal_left = true, const bool include_equal_right = true)
		{
			return in_range(correct_value - offset, correct_value + offset, value, include_equal_left, include_equal_right);
		}
		template <typename ComparableType>
		bool has_contain_part(const std::pair<ComparableType, ComparableType>& a, const std::pair<ComparableType, ComparableType>& b)
		{
			return in_range(a.first, a.second, b.first) || in_range(a.first, a.second, b.second)
				|| in_range(b.first, b.second, a.first) || in_range(b.first, b.second, a.second);
		}
	}
	namespace Time
	{
		inline std::string get_current_time(const char* format = "%Y-%m-%d_%H-%M-%S")
		{
			const auto now = std::chrono::system_clock::now();
			const auto time_t_now = std::chrono::system_clock::to_time_t(now);

			std::tm local_time;
#ifdef _WIN32
			if (localtime_s(&local_time, &time_t_now) != 0) return {};
#else
			if (!localtime_r(&time_t_now, &local_time)) return {};
#endif

			std::stringstream ss;
			ss << std::put_time(&local_time, format);
			return ss.str();
		}

		template <typename DurationType>
		long long get_duration(
			const std::chrono::time_point<std::chrono::system_clock>& previous,
			const std::chrono::time_point<std::chrono::system_clock>& current = std::chrono::system_clock::now())
		{
			return std::chrono::duration_cast<DurationType>(current - previous).count();
		}
	}
	namespace String
	{
		inline std::string trim(std::string str, const bool only_trim_right = false)
		{
			unsigned long long start = 0;
			if (!only_trim_right) for (const auto& c : str)
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
	namespace Container
	{
		template <typename Container>
		typename Container::iterator get_last_element_iterator(Container& container)
		{
			if (container.empty()) return container.end();
			return std::prev(container.end());
		}
		template <typename Container>
		typename Container::const_iterator get_last_element_iterator(const Container& container)
		{
			if (container.empty()) return container.cend();
			return std::prev(container.cend());
		}

		template <typename LeftType, typename RightType, typename Comparator>
		bool get_front_dual_queue( // đúng thì lấy bên phải (vì bên phải luôn right :>)
			const std::queue<LeftType>& left, const std::queue<RightType>& right, Comparator comp)
		{
			if (left.empty() && right.empty()) return false;
			if (right.empty()) return false;
			if (left.empty()) return true;
			return comp(left.front(), right.front());
		}

		template <typename Type>
		Type get_front_and_pop(std::queue<Type>& queue)
		{
			Type front = queue.front();
			queue.pop();
			return front;
		}
		template <typename Type>
		std::priority_queue<Type> get_front_and_pop(std::priority_queue<Type>& queue)
		{
			Type front = queue.top();
			queue.pop();
			return front;
		}

		template <typename KeyType, typename ValueType>
		std::queue<typename std::multimap<KeyType, ValueType>::value_type> multimap_to_queue(const std::multimap<KeyType, ValueType>& map)
		{
			std::queue<typename std::multimap<KeyType, ValueType>::value_type> res;
			for (const auto& item : map)
				res.push(item);
			return res;
		}
	}
}
