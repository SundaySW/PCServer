#ifndef PROTOSCLOUDSERVER_NULL_LOGGER_HPP
#define PROTOSCLOUDSERVER_NULL_LOGGER_HPP

#include "ProtosCloudServer/logging/fwd.hpp"

namespace ProtosCloudServer {

namespace logging {

/// @brief Returns a logger that drops all incoming messages
/// @see components::Logging
LoggerRef GetNullLogger() noexcept;

/// @brief Creates a logger that drops all incoming messages.
/// Use GetNullLogger() is you need a reference to logger.
LoggerPtr MakeNullLogger();

}// namespace logging
}// namespace ProtosCloudServer

#endif //PROTOSCLOUDSERVER_NULL_LOGGER_HPP