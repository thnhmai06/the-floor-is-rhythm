#pragma once
#include <cstdint>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

namespace Logging
{
	namespace Logger
	{
		constexpr std::string_view PATH = "./logs";
		void init(const std::string& name, spdlog::level::level_enum level = spdlog::level::debug,
		          uint32_t num_backtrace = 16);
	}
}

/**
 * @brief Throw exception ở level CRITICAL và log tại đúng vị trí throw ra
 * @param exception Exception muốn throw ra
 */
#define THROW_CRITICAL(exception) \
	do {\
		spdlog::log(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::critical, (exception).what()); \
		spdlog::dump_backtrace(); \
		throw (exception); \
	} while (false)

/**
 * @brief Throw exception ở level ERROR và log tại đúng vị trí throw ra
 * @param exception Exception muốn throw ra
 */
#define THROW_ERROR(exception) \
	do {\
		spdlog::log(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::err, (exception).what()); \
		spdlog::dump_backtrace(); \
		throw (exception); \
	} while (false)

/**
 * @brief Chỉ log ERROR nhưng không muốn throw exception
 * @param exception Exception muốn log ra
 */
#define LOG_ERROR(exception) \
	do {\
		try {\
			THROW_ERROR(exception);\
		} catch (...) {}\
	} while (false)

/**
 * Chỉ log WARNING nhưng không muốn throw exception
 * @param exception Exception muốn log ra
 */
#define LOG_WARNING(exception) \
	do {\
		try {\
			spdlog::log(spdlog::source_loc{ __FILE__, __LINE__, SPDLOG_FUNCTION }, spdlog::level::warn, (exception).what()); \
		} catch (...) {}\
	} while (false)

#define LOG_INFO(format, ...) \
    do {\
        try {\
            spdlog::log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, \
                        spdlog::level::info, \
                        format, __VA_ARGS__); \
        } catch (...) {}\
    } while (false)

#define LOG_DEBUG(format, ...) \
    do {\
        try {\
            spdlog::log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, \
                        spdlog::level::debug, \
                        format, __VA_ARGS__); \
        } catch (...) {}\
    } while (false)