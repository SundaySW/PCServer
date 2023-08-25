//
// Created by user on 25.08.2023.
//

#ifndef PROTOSCLOUDSERVER_LOG_HPP
#define PROTOSCLOUDSERVER_LOG_HPP

#include <atomic>

#include "ProtosCloudServer/logging/base_logger.hpp"
namespace ProtosCloudServer {

namespace logging {

using LoggerRef = ProtosCloudServer::logging::BasicLogger &;
using LoggerPtr = std::shared_ptr<ProtosCloudServer::logging::BasicLogger>;

namespace impl {
    void SetDefaultLoggerRef(LoggerRef new_logger) noexcept;
}  // namespace impl

/// @brief Returns the default logger previously set by SetDefaultLogger. If the
/// logger was not set - returns nullptr.
LoggerRef GetDefaultLogger() noexcept;

/// Sets new log level for a logger
void SetLoggerLevel(LoggerRef, Level);

/// Forces flush of default logger message queue
void LogFlush();

}// namespace logging

}// namespace ProtosCloudServer
#endif //PROTOSCLOUDSERVER_LOG_HPP
