#include "logging/logger.h" // Header
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <vector>
#include "utilities.h"

void Logging::Logger::init(const std::string& name, const spdlog::level::level_enum level, const uint32_t num_backtrace)
{
	constexpr uint16_t GRAY = 8;
	constexpr uint16_t BLUE = 1;

	constexpr std::string_view PATTERN = "(thread %t) %s:%# (%!) %^[%l] %v%$";
	const std::string log_dir = std::format("{}/{}/", PATH, Utilities::Time::get_current_time());

	const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_color(spdlog::level::debug, GRAY);
	console_sink->set_color(spdlog::level::info, BLUE);
	const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_dir + "main.log", true);
	const auto error_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_dir + "error.log", true);

	error_sink->set_level(spdlog::level::err);
	std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink, error_sink };
	const auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());

	logger->set_pattern(std::string(PATTERN));
	logger->set_level(level);
	logger->enable_backtrace(num_backtrace);

	set_default_logger(logger);
	spdlog::flush_on(spdlog::level::err);
}
