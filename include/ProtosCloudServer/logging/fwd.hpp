#ifndef PROTOSCLOUDSERVER_FWD_HPP
#define PROTOSCLOUDSERVER_FWD_HPP

/// @file ProtosCloudServer/logging/fwd.hpp
/// @brief Forward declarations for `logging` types

#include <memory>

namespace ProtosCloudServer {

namespace logging {

namespace impl {
class BaseLogger;
}

using LoggerRef = impl::BaseLogger&;
using LoggerPtr = std::shared_ptr<impl::BaseLogger>;

namespace impl {
void SetDefaultLogger(const LoggerPtr &new_logger) noexcept;

}// namespace impl
}// namespace logging
}// namespace ProtosCloudServer
#endif //PROTOSCLOUDSERVER_FWD_HPP
