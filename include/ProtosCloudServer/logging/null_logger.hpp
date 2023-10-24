#pragma once

#include "ProtosCloudServer/logging/fwd.hpp"

namespace ProtosCloudServer {

namespace logging {

LoggerRef GetNullLogger() noexcept;
LoggerPtr MakeNullLogger();

}// namespace logging
}// namespace ProtosCloudServer