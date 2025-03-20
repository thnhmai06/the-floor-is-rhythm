#pragma once
#include <cstdint>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

namespace Logging
{
	constexpr std::string_view PATH = "./logs";
	void init(std::string name, spdlog::level::level_enum level = spdlog::level::debug, uint32_t num_backtrace = 10);
}