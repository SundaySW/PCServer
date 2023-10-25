#pragma once

#include "PCServer/logging/fwd.hpp"

namespace PCServer {

namespace logging {

LoggerRef GetNullLogger() noexcept;
LoggerPtr MakeNullLogger();

}// namespace logging
}// namespace PCServer