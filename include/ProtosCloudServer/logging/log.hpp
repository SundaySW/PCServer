#ifndef PROTOSCLOUDSERVER_LOG_HPP
#define PROTOSCLOUDSERVER_LOG_HPP

#include <ProtosCloudServer/logging/level.hpp>
#include "ProtosCloudServer/logging/fwd.hpp"
#include "ProtosCloudServer/logging/log_driver.hpp"

namespace ProtosCloudServer::logging {

/// @brief Returns the default logger previously set by SetDefaultLogger. If the
/// logger was not set - returns nullptr.
LoggerRef GetDefaultLogger() noexcept;

/// Sets new log level for a logger
void SetLoggerLevel(LoggerRef, Level);

/// Forces flush of default logger message queue
void LogFlush();

/// @brief If lvl matches the verbosity then builds a stream and evaluates a
/// message for the specified logger.
#define LOG_TO(logger, lvl) LogDriver(logger, level).AsLvalue()

/// @brief If lvl matches the verbosity then builds a stream and evaluates a
/// message for the default logger.
#define LOG(lvl) LOG_TO(GetDefaultLogger(), (lvl))

/// @brief Evaluates a message and logs it to the default logger if its level is
/// below or equal to logging::Level::kDebug
#define LOG_DEBUG() LOG(Level::kDebug)

/// @brief Evaluates a message and logs it to the default logger if its level is
/// below or equal to logging::Level::kInfo
#define LOG_INFO() LOG(Level::kInfo)

/// @brief Evaluates a message and logs it to the default logger if its level is
/// below or equal to logging::Level::kWarning
#define LOG_WARNING() LOG(Level::kWarning)

/// @brief Evaluates a message and logs it to the default logger if its level is
/// below or equal to logging::Level::kError
#define LOG_ERROR() LOG(Level::kError)

/// @brief Evaluates a message and logs it to the default logger if its level is
/// below or equal to logging::Level::kCritical
#define LOG_CRITICAL() LOG(Level::kCritical)

}// namespace ProtosCloudServer::logging
#endif //PROTOSCLOUDSERVER_LOG_HPP
