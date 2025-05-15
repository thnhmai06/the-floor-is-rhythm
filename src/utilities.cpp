#include "utilities.hpp" // Header
#include <cmath>
#include <iomanip>
#include <numbers>

namespace Utilities
{
	namespace Math
	{
		bool is_equal(const float& variable, const float& value, const float& epsilon)
		{
			return abs(variable - value) <= epsilon;
		}

		bool is_integer(const float& value, const float& epsilon)
		{
			return is_equal(std::floor(value), value, epsilon);
		}

		float centre(const float& size, const float& window_size)
		{
			return (window_size - size) / 2;
		}

		SDL_FPoint centre(const SDL_FPoint& size, const SDL_FPoint& window_size)
		{
			return { .x = centre(size.x, window_size.x), .y = centre(size.y, window_size.y) };
		}

		float degree_to_radian(const float& degree)
		{
			return degree * std::numbers::pi_v<float> / 180.0f;
		}

		float radian_to_degree(const float& radian)
		{
			return radian * 180.0f / std::numbers::pi_v<float>;
		}

		double round(const double& value, const uint8_t& precision_num)
		{
			const double factor = std::pow(10.0, precision_num);
			return std::round(value * factor) / factor;
		}

		std::pair<double, double> decimal_separate(const double& value)
		{
			double integer;
			const double fraction = std::modf(value, &integer);
			return { integer, fraction };
		}

		size_t count_digits(const size_t& value)
		{
			if (value < 1) return 1;
			return static_cast<size_t>(std::log10(value)) + 1;
		}

		namespace FPoint
		{
			SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b)
			{
				return SDL_FPoint{ .x = a.x + b.x, .y = a.y + b.y };
			}

			SDL_FPoint& operator+=(SDL_FPoint& a, const SDL_FPoint& b)
			{
				a.x += b.x; a.y += b.y; return a;
			}

			SDL_FPoint operator-(const SDL_FPoint& a)
			{
				return SDL_FPoint{ .x = -a.x, .y = -a.y };
			}

			SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b)
			{
				return a + (-b);
			}

			SDL_FPoint& operator-=(SDL_FPoint& a, const SDL_FPoint& b)
			{
				return a += (-b);
			}

			SDL_FPoint operator*(const SDL_FPoint& a, const SDL_FPoint& b)
			{
				return SDL_FPoint{ .x = a.x * b.x, .y = a.y * b.y };
			}

			SDL_FPoint operator*(const SDL_FPoint& point, const float& value)
			{
				return SDL_FPoint{ .x = point.x * value, .y = point.y * value };
			}

			SDL_FPoint& operator*=(SDL_FPoint& a, const SDL_FPoint& b)
			{
				a.x *= b.x; a.y *= b.y; return a;
			}

			SDL_FPoint& operator*=(SDL_FPoint& point, const float& value)
			{
				point.x *= value; point.y *= value; return point;
			}

			SDL_FPoint operator/(const SDL_FPoint& a, const SDL_FPoint& b)
			{
				return SDL_FPoint{ .x = a.x / b.x, .y = a.y / b.y };
			}

			SDL_FPoint operator/(const SDL_FPoint& point, const float& value)
			{
				return SDL_FPoint{ .x = point.x / value, .y = point.y / value };
			}

			SDL_FPoint& operator/=(SDL_FPoint& a, const SDL_FPoint& b)
			{
				a.x /= b.x; a.y /= b.y; return a;
			}

			SDL_FPoint& operator/=(SDL_FPoint& point, const float& value)
			{
				point.x /= value; point.y /= value; return point;
			}

			bool operator==(const SDL_FPoint& a, const SDL_FPoint& b)
			{
				return  is_equal(a.x, b.x) && is_equal(a.y, b.y);
			}

			bool operator!=(const SDL_FPoint& a, const SDL_FPoint& b)
			{
				return !operator==(a, b);
			}

			SDL_Point to_integer_point(const SDL_FPoint& point)
			{
				return SDL_Point{ static_cast<int>(std::round(point.x)), static_cast<int>(std::round(point.y)) };
			}

			SDL_FPoint to_float_point(const SDL_Point& point)
			{
				return SDL_FPoint{ .x = static_cast<float>(point.x), .y = static_cast<float>(point.y) };
			}
		}
	}
	namespace Time
	{
		std::string get_current_time(const char* format)
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
	}
	namespace String
	{
		std::string trim(std::string str, const bool only_trim_right)
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

		std::vector<std::string> split(const std::string& input, char delimiter, bool only_two_part)
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
	namespace Path
	{
		fs::path normalize_path(fs::path path)
		{
			path = make_valid_long_path(path);

			return path;
		}
		fs::path make_valid_long_path(fs::path path)
		{
#ifdef _WIN32
			const std::wstring prefix = L"\\\\?\\";
			if (const auto path_str = path.wstring(); 
				!path_str.starts_with(prefix) && path_str.length() >= 260)
				path = fs::path{ prefix + std::filesystem::absolute(path).wstring() };
#endif
			return path;
		}
	}
	namespace Render
	{
		SDL_FRect merge_pos_size(const SDL_FPoint& pos, const SDL_FPoint& size)
		{
			return SDL_FRect{ pos.x, pos.y, size.x, size.y };
		}

		SDL_FPoint get_pos_from_rect(const SDL_FRect& rect)
		{
			return SDL_FPoint{ .x = rect.x, .y = rect.y };
		}

		SDL_FPoint get_size_from_rect(const SDL_FRect& rect)
		{
			return SDL_FPoint{ .x = rect.w, .y = rect.h };
		}
		bool is_point_in_rect(const SDL_FPoint& point, const SDL_FRect& rect)
		{
			return Math::in_range(rect.x, rect.x + rect.w, point.x)
				&& Math::in_range(rect.y, rect.y + rect.h, point.y);
		}
	}
}
