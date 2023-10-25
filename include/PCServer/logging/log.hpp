#pragma once

#include <PCServer/logging/level.hpp>
#include "PCServer/logging/fwd.hpp"
#include "PCServer/logging/log_driver.hpp"

namespace PCServer::logging {

LoggerRef GetDefaultLogger() noexcept;

void SetLoggerLevel(LoggerRef, Level);

void LogFlush();

/// @brief If lvl matches the verbosity then builds a stream and evaluates a
/// message for the specified logger.
#define LOG_TO(logger, lvl) LogDriver(logger, lvl).AsLvalue()

/// @brief If lvl matches the verbosity then builds a stream and evaluates a
/// message for the default logger.
#define LOG(lvl) LOG_TO(GetDefaultLogger(), (lvl))

#define LOG_DEBUG() LOG(Level::kDebug)
#define LOG_INFO() LOG(Level::kInfo)
#define LOG_WARNING() LOG(Level::kWarning)
#define LOG_ERROR() LOG(Level::kError)

}// namespace PCServer::logging
